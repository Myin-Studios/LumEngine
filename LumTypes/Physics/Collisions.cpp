#include "Collisions.h"

LumEngine::Physics::AABB LumEngine::Physics::AABB::GetBoundingBox() const
{
    AABB bounds;
    bounds.transform = this->transform;

    // Calcola il box originale
    for (const auto& vertex : vertices) {
        Vec3Core pos(vertex.Position.x, vertex.Position.y, vertex.Position.z);
        bounds.min = Vec3Core::Min(bounds.min, pos);
        bounds.max = Vec3Core::Max(bounds.max, pos);
    }

    // Crea gli 8 punti del box
    std::vector<Vec4Core> points = {
        Vec4Core(bounds.min.x(), bounds.min.y(), bounds.min.z(), 1.0f),
        Vec4Core(bounds.min.x(), bounds.min.y(), bounds.max.z(), 1.0f),
        Vec4Core(bounds.min.x(), bounds.max.y(), bounds.min.z(), 1.0f),
        Vec4Core(bounds.min.x(), bounds.max.y(), bounds.max.z(), 1.0f),
        Vec4Core(bounds.max.x(), bounds.min.y(), bounds.min.z(), 1.0f),
        Vec4Core(bounds.max.x(), bounds.min.y(), bounds.max.z(), 1.0f),
        Vec4Core(bounds.max.x(), bounds.max.y(), bounds.min.z(), 1.0f),
        Vec4Core(bounds.max.x(), bounds.max.y(), bounds.max.z(), 1.0f)
    };

    // Resetta min/max per i punti trasformati
    bounds.min = Vec3Core(std::numeric_limits<float>::infinity());
    bounds.max = Vec3Core(-std::numeric_limits<float>::infinity());

    // Trasforma tutti i punti e trova il nuovo min/max
    for (const auto& point : points) {
        Vec4Core transformed = bounds.transform * point;
        Vec3Core pos(transformed.x(), transformed.y(), transformed.z());
        bounds.min = Vec3Core::Min(bounds.min, pos);
        bounds.max = Vec3Core::Max(bounds.max, pos);
    }

    return bounds;
}

const Vec3Core LumEngine::Physics::AABB::GetMin() const
{
    // std::cout << "GetMin - Original min: " << min.ToString() << std::endl;
    // std::cout << "GetMin - Transform matrix: \n" << transform.ToString() << std::endl;

    Vec4Core minPoint(min.x(), min.y(), min.z(), 1.0f);
    // std::cout << "GetMin - Min as Vec4: " << minPoint.ToString() << std::endl;

    Vec4Core transformedMin = transform * minPoint;
    // std::cout << "GetMin - Transformed Vec4: " << transformedMin.ToString() << std::endl;

    Vec3Core result(
        transformedMin.x() / transformedMin.w(),
        transformedMin.y() / transformedMin.w(),
        transformedMin.z() / transformedMin.w()
    );
    // std::cout << "GetMin - Final result: " << result.ToString() << std::endl;

    return result;
}

const Vec3Core LumEngine::Physics::AABB::GetMax() const
{
    // Trasforma il punto massimo con la matrice di trasformazione corrente
    Vec4Core transformedMax = transform * Vec4Core(max.x(), max.y(), max.z(), 1.0f);
    return Vec3Core(
        transformedMax.x() / transformedMax.w(),
        transformedMax.y() / transformedMax.w(),
        transformedMax.z() / transformedMax.w()
    );
}

const Vec3Core LumEngine::Physics::AABB::GetCenter() const
{
	return (GetMin() + GetMax()) * 0.5f;
}

void LumEngine::Physics::AABB::SetSize(const Vec3Core& newSize)
{
	Vec3Core center = (min + max) * 0.5f;
	Vec3Core halfSize = (max - min) * 0.5f;

	min = center - newSize * 0.5f;
	max = center + newSize * 0.5f;
}

void LumEngine::Physics::AABB::SetSize(float x, float y, float z)
{
	SetSize(Vec3Core(x, y, z));
}

void LumEngine::Physics::AABB::UpdateTransform(const Mat4Core& newTransform)
{
    transform = newTransform;
}

const Mat4Core& LumEngine::Physics::AABB::GetTransform() const
{
    return transform;
}

bool LumEngine::Physics::AABB::Contains(const IAABB& bounds) const
{
	Vec3Core min = GetMin();
	Vec3Core max = GetMax();
	Vec3Core otherMin = bounds.GetMin();
	Vec3Core otherMax = bounds.GetMax();

	return min.x() <= otherMin.x() && min.y() <= otherMin.y() && min.z() <= otherMin.z() &&
		max.x() >= otherMax.x() && max.y() >= otherMax.y() && max.z() >= otherMax.z();
}

bool LumEngine::Physics::AABB::Intersects(const IRay& ray, float& distance) const
{
    Vec3Core rayOrigin = ray.GetOrigin();
    Vec3Core rayDirection = ray.GetDirection();
    Vec3Core min = GetMin();
    Vec3Core max = GetMax();

    float tmin = -std::numeric_limits<float>::infinity();
    float tmax = std::numeric_limits<float>::infinity();

    for (int i = 0; i < 3; ++i) {
        if (std::abs(rayDirection[i]) < std::numeric_limits<float>::epsilon()) {
            if (rayOrigin[i] < min[i] || rayOrigin[i] > max[i])
                return false;
        }
        else {
            float invD = 1.0f / rayDirection[i];
            float t1 = (min[i] - rayOrigin[i]) * invD;
            float t2 = (max[i] - rayOrigin[i]) * invD;
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

float LumEngine::Physics::AABB::GetDistanceToRay(const IRay& origin) const
{
	float distance;
	if (origin.IntersectsAABB(*this, distance)) {
		return distance;
	}

	return -1.0f;
}

float LumEngine::Physics::AABB::GetDistanceToPoint(const Vec3Core& point) const
{
	Vec3Core toPoint = point - GetCenter();
    float projectionLength = toPoint * Vec3Core(0, 0, 1);

	if (projectionLength < 0) {
		return toPoint.Length();
	}

	Vec3Core projection = Vec3Core(0, 0, 1) * projectionLength;
	return (toPoint - projection).Length();
}

Mat4Core LumEngine::Physics::RayCast::projectionMatrix = Mat4Core();
Mat4Core LumEngine::Physics::RayCast::viewMatrix = Mat4Core();
Vec3Core LumEngine::Physics::RayCast::origin = Vec3Core();
Vec3Core LumEngine::Physics::RayCast::direction = Vec3Core();
std::vector<std::shared_ptr<LumEngine::Physics::AABB>> LumEngine::Physics::RayCast::boundingVolumes = {};

LumEngine::Physics::RayCastResult LumEngine::Physics::RayCast::Cast(const Vec3Core& origin, const Vec3Core& direction) {
    RayCastResult result = { false, std::numeric_limits<float>::max(), -1, Vec3Core() };

    // Invece di ordinare, possiamo tenere traccia solo del più vicino
    for (const auto& bounds : boundingVolumes) {
        float distance;
        Vec3Core hitPoint;

        if (IntersectRayBounds(origin, direction, bounds, distance, hitPoint)) {
            if (distance < result.distance) {
                result.hit = true;
                result.distance = distance;
                result.hitPoint = hitPoint;
                result.entityId = bounds->GetEntityID();
            }
        }
    }

    return result;
}

Vec3Core LumEngine::Physics::RayCast::ScreenToRay(int mouseX, int mouseY, int width, int height)
{
    float x = (2.0f * mouseX) / width - 1.0f;
    float y = 1.0f - (2.0f * mouseY) / height;

    Vec4Core nearPoint(x, y, -1.0f, 1.0f);
    Vec4Core farPoint(x, y, 1.0f, 1.0f);

    Mat4Core viewProj = projectionMatrix * viewMatrix;

    // Controlla il determinante prima dell'inversione
    float det = viewProj.Determinant();
    if (std::abs(det) < 1e-6) {
        std::cout << "Warning: Matrix close to singular!" << std::endl;
    }

    Mat4Core invVP = viewProj.Inverse();

    Mat4Core identityTest = viewProj * invVP;

    Vec4Core nearWorld = invVP * nearPoint;

    Vec4Core farWorld = invVP * farPoint;

    nearWorld = nearWorld / nearWorld.w();
    farWorld = farWorld / farWorld.w();

    Vec3Core resultDirection = (Vec3Core(farWorld - nearWorld)).Normalize();

    return resultDirection;
}

bool LumEngine::Physics::RayCast::IntersectRayBounds(const Vec3Core& origin, const Vec3Core& direction, std::shared_ptr<AABB> bounds, float& distance, Vec3Core& hitPoint)
{
    Vec3Core bMin = bounds->GetMin();
    Vec3Core bMax = bounds->GetMax();

    const float EPSILON = 1e-6f;
    float dirX = std::abs(direction.x()) < EPSILON ? EPSILON : direction.x();
    float dirY = std::abs(direction.y()) < EPSILON ? EPSILON : direction.y();
    float dirZ = std::abs(direction.z()) < EPSILON ? EPSILON : direction.z();

    float t1 = (bMin.x() - origin.x()) / dirX;
    float t2 = (bMax.x() - origin.x()) / dirX;
    float t3 = (bMin.y() - origin.y()) / dirY;
    float t4 = (bMax.y() - origin.y()) / dirY;
    float t5 = (bMin.z() - origin.z()) / dirZ;
    float t6 = (bMax.z() - origin.z()) / dirZ;

    float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
    float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

    if (tmax < tmin || tmax < 0) {
        std::cout << "No intersection" << std::endl;
        return false;
    }

    distance = tmin >= 0 ? tmin : tmax;
    hitPoint = origin + direction * distance;
    // std::cout << "Hit at distance=" << distance << " point=" << hitPoint.ToString() << std::endl;

    return true;
}

void LumEngine::Physics::RayCast::SetProjectionMatrix(const Mat4Core& projection)
{
    projectionMatrix = projection;
}

void LumEngine::Physics::RayCast::SetViewMatrix(const Mat4Core& view)
{
    viewMatrix = view;
}

void LumEngine::Physics::RayCast::RegisterBoundingVolume(std::vector<Vertex> verts)
{
    Vec3Core min(std::numeric_limits<float>::infinity());
    Vec3Core max(-std::numeric_limits<float>::infinity());

    for (const auto& vertex : verts) {
        min.setX(std::min(min.x(), vertex.Position.x));
        min.setY(std::min(min.y(), vertex.Position.y));
        min.setZ(std::min(min.z(), vertex.Position.z));

        max.setX(std::max(max.x(), vertex.Position.x));
        max.setY(std::max(max.y(), vertex.Position.y));
        max.setZ(std::max(max.z(), vertex.Position.z));
    }

    std::shared_ptr<AABB> newBounds = std::make_shared<AABB>(min, max, boundingVolumes.size(), verts);
    boundingVolumes.push_back(newBounds);
}

void LumEngine::Physics::RayCast::UnregisterBoundingVolume(std::vector<Vertex> verts, int entityId)
{
    boundingVolumes.erase(
        std::remove_if(boundingVolumes.begin(), boundingVolumes.end(),
            [entityId](std::shared_ptr<AABB> bounds) {
                return bounds->GetEntityID() == entityId;
            }),
        boundingVolumes.end()
    );
}

void LumEngine::Physics::RayCast::UpdateTransform(int entityID, const Mat4Core& transform)
{
    auto it = std::find_if(boundingVolumes.begin(), boundingVolumes.end(),
        [entityID](const std::shared_ptr<AABB>& bounds) {
            return bounds->GetEntityID() == entityID;
        });

    if (it != boundingVolumes.end()) {
        (*it)->UpdateTransform(transform);
    }
}

LumEngine::Physics::Collider::Collider(int eID, const std::vector<Vertex>& verts, Mat4Core& t) : vertices(verts)
, entityId(eID), transform(t)
{
    _boundingBox = AABB(vertices);
    SetSize(1.0f, 1.0f, 1.0f);
}

const LumEngine::Physics::AABB& LumEngine::Physics::Collider::GetBoundingBox() const
{
    if (_needUpdate) {
		_boundingBox = CalculateBoundingBox();
		_needUpdate = false;
	}
	return _boundingBox;
}

int LumEngine::Physics::Collider::GetEntityID() const { return entityId; }
const std::vector<Vertex>& LumEngine::Physics::Collider::GetVertices() const { return vertices; }

void LumEngine::Physics::Collider::SetSize(const Vec3Core& newSize)
{
	_boundingBox.SetSize(newSize);
}

void LumEngine::Physics::Collider::SetSize(float x, float y, float z)
{
	_boundingBox.SetSize(Vec3Core(x, y, z));
}

Vec3Core LumEngine::Physics::Collider::GetSize() const
{
	return _boundingBox.GetMax() - _boundingBox.GetMin();
}

ColliderType LumEngine::Physics::Collider::GetColliderType() const
{
    return type;
}

void LumEngine::Physics::Collider::SetColliderType(ColliderType newType)
{
	type = newType;
}

void LumEngine::Physics::Collider::UpdateTransform(const Mat4Core& newTransform)
{
    _boundingBox.UpdateTransform(newTransform);
}

void LumEngine::Physics::Collider::InvalidateBoundingBox() const
{
	_needUpdate = true;
}

LumEngine::Physics::AABB LumEngine::Physics::Collider::CalculateBoundingBox() const
{
    return _boundingBox.GetBoundingBox();
}
