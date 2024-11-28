#include "Collisions.h"

LumEngine::Physics::AABB LumEngine::Physics::AABB::GetBoundingBox() const
{
    AABB bounds;
    // Inizializza con valori estremi
    bounds.min = Vec3Core(
        std::numeric_limits<float>::max(),
        std::numeric_limits<float>::max(),
        std::numeric_limits<float>::max()
    );
    bounds.max = Vec3Core(
        std::numeric_limits<float>::lowest(),
        std::numeric_limits<float>::lowest(),
        std::numeric_limits<float>::lowest()
    );

    // Calcola AABB dai vertici della mesh
    for (const auto& vertex : vertices) {
        // Qui usiamo i valori direttamente invece di std::min/max
        bounds.min.setX((bounds.min.x() < vertex.Position.x) ? bounds.min.x() : vertex.Position.x);
        bounds.min.setY((bounds.min.y() < vertex.Position.y) ? bounds.min.y() : vertex.Position.y);
        bounds.min.setZ((bounds.min.z() < vertex.Position.z) ? bounds.min.z() : vertex.Position.z);

        bounds.max.setX((bounds.max.x() > vertex.Position.x) ? bounds.max.x() : vertex.Position.x);
        bounds.max.setY((bounds.max.y() > vertex.Position.y) ? bounds.max.y() : vertex.Position.y);
        bounds.max.setZ((bounds.max.z() > vertex.Position.z) ? bounds.max.z() : vertex.Position.z);
    }

    return bounds;
}

Mat4Core LumEngine::Physics::RayCast::projectionMatrix = Mat4Core();
Mat4Core LumEngine::Physics::RayCast::viewMatrix = Mat4Core();
Vec3Core LumEngine::Physics::RayCast::origin = Vec3Core();
Vec3Core LumEngine::Physics::RayCast::direction = Vec3Core();
std::vector<std::shared_ptr<LumEngine::Physics::AABB>> LumEngine::Physics::RayCast::boundingVolumes = {};

LumEngine::Physics::RayCastResult LumEngine::Physics::RayCast::Cast(const Vec3Core& origin, const Vec3Core& direction)
{
    RayCastResult result = { false, std::numeric_limits<float>::max(), -1, Vec3Core() };

    for (const auto& bounds : boundingVolumes) {
        float distance;
        Vec3Core hitPoint;

        if (IntersectRayBounds(origin, direction, bounds, distance, hitPoint)) {
            if (distance < result.distance) {
                result.hit = true;  
                result.distance = distance;
                result.hitPoint = hitPoint;
            }
        }
    }

    return result;
}

void LumEngine::Physics::RayCast::ScreenToRay(int mouseX, int mouseY, int width, int height, Vec3Core& origin, Vec3Core& direction)
{
    float x = (2.0f * mouseX) / width - 1.0f;
    float y = 1.0f - (2.0f * mouseY) / height;

    std::cout << "Mouse NDC: x=" << x << " y=" << y << std::endl;

    Vec4Core nearPoint(x, y, -1.0f, 1.0f);
    Vec4Core farPoint(x, y, 1.0f, 1.0f);

    std::cout << "Near point: " << nearPoint.ToString() << std::endl;
    std::cout << "Far point: " << farPoint.ToString() << std::endl;

    Mat4Core viewProj = projectionMatrix * viewMatrix;
    std::cout << "ViewProj Matrix:" << std::endl << viewProj.ToString() << std::endl;

    // Controlla il determinante prima dell'inversione
    float det = viewProj.Determinant();
    std::cout << "Matrix determinant: " << det << std::endl;
    if (std::abs(det) < 1e-6) {
        std::cout << "Warning: Matrix close to singular!" << std::endl;
    }

    Mat4Core invVP = viewProj.Inverse();

	std::cout << "Inverse ViewProj Matrix:" << std::endl << invVP.ToString() << std::endl;

    Mat4Core identityTest = viewProj * invVP;

	std::cout << "Identity test:" << std::endl << identityTest.ToString() << std::endl;

    Vec4Core nearWorld = invVP * nearPoint;

    std::cout << "Near world before: " << nearWorld.ToString() << std::endl;

    Vec4Core farWorld = invVP * farPoint;

    std::cout << "Far world before: " << farWorld.ToString() << std::endl;

    nearWorld = nearWorld / nearWorld.w();
    farWorld = farWorld / farWorld.w();

    std::cout << "Near world after: " << nearWorld.ToString() << std::endl;
    std::cout << "Far world after: " << farWorld.ToString() << std::endl;

    origin = Vec3Core(nearWorld);
    Vec3Core resultDirection = (Vec3Core(farWorld - nearWorld)).Normalize();

    direction = resultDirection;

    std::cout << "Ray origin: " << origin.ToString() << std::endl;
    std::cout << "Ray direction: " << direction.ToString() << std::endl;

    std::cout << std::endl;
}

bool LumEngine::Physics::RayCast::IntersectRayBounds(const Vec3Core& origin, const Vec3Core& direction, std::shared_ptr<AABB> bounds, float& distance, Vec3Core& hitPoint)
{
    float tmin = (bounds->GetMin().x() - origin.x()) / direction.x();
    float tmax = (bounds->GetMax().x() - origin.x()) / direction.x();

    if (tmin > tmax) std::swap(tmin, tmax);

    float tymin = (bounds->GetMin().y() - origin.y()) / direction.y();
    float tymax = (bounds->GetMax().y() - origin.y()) / direction.y();

    if (tymin > tymax) std::swap(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax))
        return false;

    if (tymin > tmin) tmin = tymin;
    if (tymax < tmax) tmax = tymax;

    float tzmin = (bounds->GetMin().z() - origin.z()) / direction.z();
    float tzmax = (bounds->GetMax().z() - origin.z()) / direction.z();

    if (tzmin > tzmax) std::swap(tzmin, tzmax);

    if ((tmin > tzmax) || (tzmin > tmax))
        return false;

    if (tzmin > tmin) tmin = tzmin;
    if (tzmax < tmax) tmax = tzmax;

    distance = tmin;
    hitPoint = Vec3Core(
        origin.x() + direction.x() * distance,
        origin.y() + direction.y() * distance,
        origin.z() + direction.z() * distance
    );
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
    std::shared_ptr<AABB> newBounds = std::make_shared<AABB>(AABB(verts).GetBoundingBox());
    std::cout << "Registering AABB:" << std::endl;
    std::cout << "Min: " << newBounds->GetMin().ToString() << std::endl;
    std::cout << "Max: " << newBounds->GetMax().ToString() << std::endl;
    boundingVolumes.push_back(newBounds);
    std::cout << "Total bounding volumes: " << boundingVolumes.size() << std::endl;
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

LumEngine::Physics::Collider::Collider(int eID, const std::vector<Vertex>& verts) : vertices(verts)
, entityId(eID)
{
    boundingBox = AABB(vertices);
}

const LumEngine::Physics::AABB& LumEngine::Physics::Collider::GetBoundingBox() const { return boundingBox; }
int LumEngine::Physics::Collider::GetEntityID() const { return entityId; }
const std::vector<Vertex>& LumEngine::Physics::Collider::GetVertices() const { return vertices; }