#pragma once

#include "Collisions.h"

#include <algorithm>
#include <array>

namespace LumEngine::Physics
{
    class Ray : public IRay {
    private:
        Vec3Core _origin;
        Vec3Core _direction;

    public:
        Ray(const Vec3Core& origin, const Vec3Core& direction)
            : _origin(origin), _direction(direction)
        {
			_direction.Normalize();
        }

        // Getters
        const Vec3Core& GetOrigin() const { return _origin; }
        const Vec3Core& GetDirection() const { return _direction; }

        // Ottieni un punto lungo il raggio a una certa distanza
        Vec3Core GetPoint(float distance) const {
            return _origin + (_direction * distance);
        }

        // Calcola la distanza minima tra il raggio e un punto
        float GetDistanceToPoint(const Vec3Core& point) const {
            Vec3Core toPoint = point - _origin;
            float projectionLength = toPoint * _direction;

            if (projectionLength < 0) {
                return toPoint.Length();  // Il punto è dietro l'origine
            }

            Vec3Core projection = _direction * projectionLength;
            return (toPoint - projection).Length();
        }

        // Test di intersezione con AABB
        bool IntersectsAABB(const IAABB& aabb, float& distance) const override {
            Vec3Core min = aabb.GetMin();
            Vec3Core max = aabb.GetMax();

            float tmin = -std::numeric_limits<float>::infinity();
            float tmax = std::numeric_limits<float>::infinity();

            for (int i = 0; i < 3; ++i) {
                if (std::abs(_direction[i]) < std::numeric_limits<float>::epsilon()) {
                    // Raggio parallelo all'asse
                    if (_origin[i] < min[i] || _origin[i] > max[i])
                        return false;
                }
                else {
                    float invD = 1.0f / _direction[i];
                    float t1 = (min[i] - _origin[i]) * invD;
                    float t2 = (max[i] - _origin[i]) * invD;

                    if (t1 > t2) std::swap(t1, t2);

                    tmin = t1 > tmin ? t1 : tmin;
                    tmax = t2 < tmax ? t2 : tmax;

                    if (tmin > tmax) return false;
                }
            }

            if (tmin < 0) {
                distance = tmax;
                return tmax > 0;
            }

            distance = tmin;
            return true;
        }

        // Operatore di accesso agli elementi per compatibilità con Vec3Core
        float operator[](int i) const {
            return i == 0 ? _direction.x() : (i == 1 ? _direction.y() : _direction.z());
        }
    };

    class Octree {
    private:
        struct Node {
            AABB bounds;
            std::vector<BaseEntity*> entities;
            std::array<std::unique_ptr<Node>, 8> children;
            static const int MAX_ENTITIES = 8;  // Soglia per la suddivisione
            bool isLeaf = true;
        };

        std::unique_ptr<Node> root;

    public:
        Octree(const AABB& worldBounds) {
            root = std::make_unique<Node>();
            root->bounds = worldBounds;
        }

        void Insert(BaseEntity* entity) {
            InsertIntoNode(root.get(), entity);
        }

        void Remove(BaseEntity* entity) {
			// TODO: Implement
        }

        BaseEntity* RayCast(const Vec3Core& origin, const Vec3Core& direction) {
            Ray ray(origin, direction);
            float nearestDist = std::numeric_limits<float>::max();
            BaseEntity* nearestEntity = nullptr;
            QueryRay(root.get(), ray, nearestDist, nearestEntity);
            return nearestEntity;
        }

    private:
        void InsertIntoNode(Node* node, BaseEntity* entity) {
            if (!node || !entity->GetCoreProperty<const LumEngine::Physics::Collider>()) return;

            auto collider = entity->GetCoreProperty<const LumEngine::Physics::Collider>();
            const IAABB& bounds = collider->GetBoundingBox();

            if (!node->bounds.Contains(bounds)) {
                return;
            }

            if (node->isLeaf && node->entities.size() < Node::MAX_ENTITIES) {
                node->entities.push_back(entity);
                return;
            }

            Vec3Core nodeSize = node->bounds.GetMax() - node->bounds.GetMin();
			std::cout << "Node size: " << nodeSize.ToString() << std::endl;

            // Se pieno, suddividi
            if (node->isLeaf) {
                SubdivideNode(node);
            }

            // Usa il bounding box invece della posizione
            Vec3Core center = (bounds.GetMin() + bounds.GetMax()) * 0.5f;
            Vec3Core nodeCenter = node->bounds.GetCenter();

            std::cout << "Bounds: " << bounds.GetMin().ToString() << " - " << bounds.GetMax().ToString() << std::endl;

            int octant = 0;
            if (center.x() >= nodeCenter.x()) octant |= 4;
            if (center.y() >= nodeCenter.y()) octant |= 2;
            if (center.z() >= nodeCenter.z()) octant |= 1;

            InsertIntoNode(node->children[octant].get(), entity);
        }

        void SubdivideNode(Node* node) {
            Vec3Core center = node->bounds.GetCenter();
            Vec3Core halfSize = (node->bounds.GetMax() - node->bounds.GetMin()) * 0.5f;

            // Crea gli 8 nodi figli
            for (int i = 0; i < 8; i++) {
                Vec3Core min, max;
                min.setX((i & 4) ? center.x() : node->bounds.GetMin().x());
                min.setY((i & 2) ? center.y() : node->bounds.GetMin().y());
                min.setZ((i & 1) ? center.z() : node->bounds.GetMin().z());

                max.setX((i & 4) ? node->bounds.GetMax().x() : center.x());
                max.setY((i & 2) ? node->bounds.GetMax().y() : center.y());
                max.setZ((i & 1) ? node->bounds.GetMax().z() : center.z());

                node->children[i] = std::make_unique<Node>();
                node->children[i]->bounds = AABB(min, max);
            }

            // Redistribuisci le entità esistenti
            auto entities = std::move(node->entities);
            node->isLeaf = false;

            for (auto* entity : entities) {
                InsertIntoNode(node, entity);
            }
        }

        void QueryRay(Node* node, const Ray& ray, float& nearestDist, BaseEntity*& nearestEntity) {
            if (!node) {
                std::cout << "Node is null" << std::endl;
                return;
            }

            float nodeDist = std::numeric_limits<float>::max();
            if (!node->bounds.Intersects(ray, nodeDist)) {
                std::cout << "Ray does not intersect node bounds" << std::endl;
                return;
            }

            if (node->isLeaf) {
                std::cout << "Leaf node contains " << node->entities.size() << " entities" << std::endl;
                for (auto* entity : node->entities) {
                    float dist = std::numeric_limits<float>::max();
                    if (entity->Intersects(ray, dist)) {
                        std::cout << "Hit entity " << entity->GetEntityID() << " at distance " << dist << std::endl;
                        if (dist < nearestDist) {
                            nearestDist = dist;
                            nearestEntity = entity;
                        }
                    }
                    else {
                        std::cout << "Entity " << entity->GetEntityID() << " not hit" << std::endl;
                    }
                }
                return;
            }

            std::cout << "Non-leaf node, checking children" << std::endl;

            std::array<std::pair<float, Node*>, 8> sortedChildren;
            for (int i = 0; i < 8; i++) {
                if (node->children[i]) {
                    float dist = node->children[i]->bounds.GetDistanceToPoint(ray.GetOrigin());
                    sortedChildren[i] = { dist, node->children[i].get() };

					std::cout << "Child " << i << " at distance " << dist << std::endl;
                }
            }

            std::sort(sortedChildren.begin(), sortedChildren.end());

            // Visita i nodi in ordine di distanza
            for (const auto& [dist, child] : sortedChildren) {
                if (child) QueryRay(child, ray, nearestDist, nearestEntity);

				std::cout << "Visited child at distance " << dist << std::endl;
            }
        }
    };
}