#pragma once

#include "Engine/GUI/Rendering/Vertices/Vertices.h"
#include "Engine/GUI/Rendering/Meshes/Mesh.h"
#include "../Mathematics/Math.h"
#include "../Entities/Properties/Property.h"
#include "../Entities/Entity.h"
#include <algorithm>
#include <vector>

namespace LumEngine::Physics
{
    class AABB : public IAABB
    {
    public:
        AABB(std::vector<Vertex> verts = {}) : vertices(std::move(verts))
        {
            transform = Mat4Core::Identity();
        }

        AABB(const Vec3Core& min, const Vec3Core& max, int id, const std::vector<Vertex>& verts)
            : min(min), max(max), entityId(id), vertices(verts)
        {
            transform = Mat4Core::Identity();
        }

		AABB(const Vec3Core& min, const Vec3Core& max) : min(min), max(max)
		{
			transform = Mat4Core::Identity();
		}

        AABB GetBoundingBox() const;

        const Vec3Core GetMin() const override;
        const Vec3Core GetMax() const override;
        const int GetEntityID() const override { return entityId; }

		const Vec3Core GetCenter() const override;

		void SetSize(const Vec3Core& newSize) override;
		void SetSize(float x, float y, float z) override;

        void UpdateTransform(const Mat4Core& newTransform) override;
        const Mat4Core& GetTransform() const override;

		float GetDistanceToRay(const IRay& ray) const;

		float GetDistanceToPoint(const Vec3Core& origin) const override;

		bool Contains(const IAABB& bounds) const override;

        bool Intersects(const IRay& ray, float& distance) const;

    private:
        std::vector<Vertex> vertices;
        Mat4Core transform;
        int entityId = 0;
        Vec3Core min;
        Vec3Core max;
    };

    struct RayCastResult {
        bool hit;
        float distance;
        int entityId;
        Vec3Core hitPoint;
    };

    class RayCast
    {
    public:

        static RayCastResult Cast(const Vec3Core& origin, const Vec3Core& direction);
        static Vec3Core ScreenToRay(int mouseX, int mouseY, int width, int height);
        static bool IntersectRayBounds(const Vec3Core& origin, const Vec3Core& direction,
            std::shared_ptr<AABB> bounds, float& distance, Vec3Core& hitPoint);

        static void SetProjectionMatrix(const Mat4Core& projection);
        static void SetViewMatrix(const Mat4Core& view);

        static void RegisterBoundingVolume(std::vector<Vertex> verts);
        static void UnregisterBoundingVolume(std::vector<Vertex> verts, int entityId);

        static void UpdateTransform(int entityID, const Mat4Core& transform);

    private:
        static Mat4Core projectionMatrix;
        static Mat4Core viewMatrix;
        static Vec3Core origin;
        static Vec3Core direction;

        static std::vector<std::shared_ptr<AABB>> boundingVolumes;
    };

    class Collider : public ICollider
    {
		mutable AABB _boundingBox;
		mutable bool _needUpdate = true;
		std::weak_ptr<MeshCore> _mesh;
        Transform3DCore* _transform;

    public:
        Collider(int eID, const std::vector<Vertex>& verts, Mat4Core& t);

		void InvalidateBoundingBox() const;

		AABB CalculateBoundingBox() const;

        const AABB& GetBoundingBox() const override;
        int GetEntityID() const override;
        const std::vector<Vertex>& GetVertices() const override;

        virtual void OnSerialize() override {}
        virtual void OnDeserialize() override {}

		void SetSize(const Vec3Core& newSize) override;
		void SetSize(float x, float y, float z) override;

		Vec3Core GetSize() const override;

		ColliderType GetColliderType() const override;
		void SetColliderType(ColliderType newType) override;

        void UpdateTransform(const Mat4Core& newTransform) override;

    private:
        std::vector<Vertex> vertices;
        Mat4Core& transform;
        int entityId;
		ColliderType type;
    };
}