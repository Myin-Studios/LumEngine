#pragma once

#include "../Mathematics/Math.h"
#include "../Entities/Properties/Property.h"
#include "../LumEngine/Engine/GUI/Rendering/Vertices/Vertices.h"
#include "../Entities/Entity.h"
#include <algorithm>
#include <vector>

namespace LumEngine::Physics
{
    class AABB
    {
    public:
        AABB(std::vector<Vertex> verts = {}) : vertices(std::move(verts))
        {

        }

        AABB(const Vec3Core& min, const Vec3Core& max, int id, const std::vector<Vertex>& verts)
            : min(min), max(max), entityId(id), vertices(verts) {}

        AABB GetBoundingBox() const;

        const Vec3Core GetMin() const { return min; }
        const Vec3Core GetMax() const { return max; }
        const int GetEntityID() const { return entityId; }

    private:
        std::vector<Vertex> vertices;
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

        static RayCastResult Cast(Vec3Core& origin, Vec3Core& direction);
        static void ScreenToRay(int mouseX, int mouseY, int width, int height, Vec3Core& origin, Vec3Core& direction);
        static bool IntersectRayBounds(const Vec3Core& origin, const Vec3Core& direction,
            const AABB& bounds, float& distance, Vec3Core& hitPoint);

        static void SetProjectionMatrix(const Mat4Core& projection);
        static void SetViewMatrix(const Mat4Core& view);

        static void RegisterBoundingVolume(std::vector<Vertex> verts);
        static void UnregisterBoundingVolume(std::vector<Vertex> verts, int entityId);
    private:
        static Mat4Core projectionMatrix;
        static Mat4Core viewMatrix;

        static std::vector<AABB> boundingVolumes;
    };

    class Collider : public IProperty
    {
    public:
        Collider(int eID, const std::vector<Vertex>& verts);

        const AABB& GetBoundingBox() const;
        int GetEntityID() const;
        const std::vector<Vertex>& GetVertices() const;

        virtual void OnSerialize() override {}
        virtual void OnDeserialize() override {}

    private:
        std::vector<Vertex> vertices;
        AABB boundingBox;
        int entityId;
    };
}