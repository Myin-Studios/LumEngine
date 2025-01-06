#pragma once

#include "../Mathematics/Math.h"
#include "Engine/GUI/Rendering/Vertices/Vertices.h"
#include "../Entities/Properties/Property.h"

enum ColliderType
{
	AABB,
	Sphere,
	Capsule,
	Mesh
};

namespace LumEngine::Physics
{
	class IAABB
	{
	public:
		virtual ~IAABB() = default;

		virtual const Vec3Core GetMin() const = 0;
		virtual const Vec3Core GetMax() const = 0;
		virtual const int GetEntityID() const = 0;

		virtual const Vec3Core GetCenter() const = 0;

		virtual void SetSize(const Vec3Core& newSize) = 0;
		virtual void SetSize(float x, float y, float z) = 0;

		virtual void UpdateTransform(const Mat4Core& newTransform) = 0;
		virtual const Mat4Core& GetTransform() const = 0;

		virtual float GetDistanceToPoint(const Vec3Core& origin) const = 0;

		virtual bool Contains(const IAABB& bounds) const = 0;

	};

	class IRay
	{
	public:
		virtual ~IRay() = default;

		virtual const Vec3Core& GetOrigin() const = 0;
		virtual const Vec3Core& GetDirection() const = 0;

		virtual Vec3Core GetPoint(float distance) const = 0;
		virtual float GetDistanceToPoint(const Vec3Core& point) const = 0;

		virtual bool IntersectsAABB(const IAABB& aabb, float& distance) const = 0;
	};

	class ICollider : public IProperty
	{
	public:
		virtual ~ICollider() = default;

		virtual const IAABB& GetBoundingBox() const = 0;
		virtual int GetEntityID() const = 0;
		virtual const std::vector<Vertex>& GetVertices() const = 0;

		virtual void SetSize(const Vec3Core& newSize) = 0;
		virtual void SetSize(float x, float y, float z) = 0;
		virtual Vec3Core GetSize() const = 0;

		virtual ColliderType GetColliderType() const = 0;
		virtual void SetColliderType(ColliderType newType) = 0;

		virtual void UpdateTransform(const Mat4Core& newTransform) = 0;
	};
}