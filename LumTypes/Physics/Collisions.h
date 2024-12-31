#pragma once

#include "../Mathematics/Math.h"
#include "../Entities/Properties/Property.h"
#include "../LumEngine/Engine/GUI/Rendering/Vertices/Vertices.h"
#include "../Entities/Entity.h"
#include <algorithm>
#include <vector>

enum ColliderType
{
    AABB,
    Sphere,
    Capsule,
    Mesh
};

namespace LumEngine::Physics
{
    class AABB
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

        AABB GetBoundingBox() const;

        const Vec3Core GetMin() const;
        const Vec3Core GetMax() const;
        const int GetEntityID() const { return entityId; }

		void SetSize(const Vec3Core& newSize);
		void SetSize(float x, float y, float z);

        void UpdateTransform(const Mat4Core& newTransform);
        const Mat4Core& GetTransform() const;

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

    class Collider : public IProperty
    {
    public:
        Collider(int eID, const std::vector<Vertex>& verts, Mat4Core& t);

        const AABB& GetBoundingBox() const;
        int GetEntityID() const;
        const std::vector<Vertex>& GetVertices() const;

        virtual void OnSerialize() override {}
        virtual void OnDeserialize() override {}

		void SetSize(const Vec3Core& newSize);
		void SetSize(float x, float y, float z);

		Vec3Core GetSize() const;

		ColliderType GetColliderType() const;
		void SetColliderType(ColliderType newType);

        void UpdateTransform(const Mat4Core& newTransform);

    private:
        std::vector<Vertex> vertices;
        Mat4Core& transform;
        AABB boundingBox;
        int entityId;
		ColliderType type;
    };
}