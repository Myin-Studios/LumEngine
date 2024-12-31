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

Mat4Core LumEngine::Physics::RayCast::projectionMatrix = Mat4Core();
Mat4Core LumEngine::Physics::RayCast::viewMatrix = Mat4Core();
Vec3Core LumEngine::Physics::RayCast::origin = Vec3Core();
Vec3Core LumEngine::Physics::RayCast::direction = Vec3Core();
std::vector<std::shared_ptr<LumEngine::Physics::AABB>> LumEngine::Physics::RayCast::boundingVolumes = {};

LumEngine::Physics::RayCastResult LumEngine::Physics::RayCast::Cast(const Vec3Core& origin, const Vec3Core& direction)
{
    RayCastResult result = { false, std::numeric_limits<float>::max(), -1, Vec3Core() };

    // Ordina i boundingVolumes per distanza dall'origine del raggio
    std::vector<std::pair<float, std::shared_ptr<AABB>>> sortedVolumes;
    for (const auto& bounds : boundingVolumes) {
        // Ottieni il bounding box trasformato
        std::cout << "Pre-sort bounds transform:\n" << bounds->GetTransform().ToString() << std::endl;
        Vec3Core center = (bounds->GetMin() + bounds->GetMax()) * 0.5f;
        float dist = (center - origin).Length();
        sortedVolumes.push_back({ dist, bounds });
    }

    std::sort(sortedVolumes.begin(), sortedVolumes.end(),
        [](const auto& a, const auto& b) { return a.first < b.first; });

    for (const auto& [dist, bounds] : sortedVolumes) {
        std::cout << "Post-sort bounds transform:\n" << bounds->GetTransform().ToString() << std::endl;
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
    // std::cout << "\n\n=== Testing intersection ===" << std::endl;

    // Ottieni i punti trasformati
    Vec3Core bMin = bounds->GetMin();
    Vec3Core bMax = bounds->GetMax();

    // std::cout << "Current transform in IntersectRayBounds:\n" << bounds->GetTransform().ToString() << std::endl;

    // std::cout << "Ray test with:" << std::endl;
    // std::cout << "Origin: " << origin.ToString() << std::endl;
    // std::cout << "Direction: " << direction.ToString() << std::endl;
    // std::cout << "Transformed min: " << bMin.ToString() << std::endl;
    // std::cout << "Transformed max: " << bMax.ToString() << std::endl;

    const float EPSILON = 1e-6f;
    float dirX = std::abs(direction.x()) < EPSILON ? EPSILON : direction.x();
    float dirY = std::abs(direction.y()) < EPSILON ? EPSILON : direction.y();
    float dirZ = std::abs(direction.z()) < EPSILON ? EPSILON : direction.z();

    // Calcola i tempi di intersezione
    float t1 = (bMin.x() - origin.x()) / dirX;
    float t2 = (bMax.x() - origin.x()) / dirX;
    float t3 = (bMin.y() - origin.y()) / dirY;
    float t4 = (bMax.y() - origin.y()) / dirY;
    float t5 = (bMin.z() - origin.z()) / dirZ;
    float t6 = (bMax.z() - origin.z()) / dirZ;

    // std::cout << "Intersection times:" << std::endl;
    // std::cout << "X: t1=" << t1 << " t2=" << t2 << std::endl;
    // std::cout << "Y: t3=" << t3 << " t4=" << t4 << std::endl;
    // std::cout << "Z: t5=" << t5 << " t6=" << t6 << std::endl;

    float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
    float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

    // std::cout << "tmin=" << tmin << " tmax=" << tmax << std::endl;

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
    boundingBox = AABB(vertices);
    SetSize(1.0f, 1.0f, 1.0f);
}

const LumEngine::Physics::AABB& LumEngine::Physics::Collider::GetBoundingBox() const { return boundingBox; }
int LumEngine::Physics::Collider::GetEntityID() const { return entityId; }
const std::vector<Vertex>& LumEngine::Physics::Collider::GetVertices() const { return vertices; }

void LumEngine::Physics::Collider::SetSize(const Vec3Core& newSize)
{
	boundingBox.SetSize(newSize);
}

void LumEngine::Physics::Collider::SetSize(float x, float y, float z)
{
	boundingBox.SetSize(Vec3Core(x, y, z));
}

Vec3Core LumEngine::Physics::Collider::GetSize() const
{
	return boundingBox.GetBoundingBox().GetMax() - boundingBox.GetBoundingBox().GetMin();
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
    boundingBox.UpdateTransform(newTransform);
}