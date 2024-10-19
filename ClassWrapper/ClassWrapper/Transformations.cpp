#include "Transformations.h"

Transformations::Transform3D()
{
    _transform = new Transform3DCore();
}

Transformations::~Transform3D() {
    this->!Transform3D(); // Chiama il finalizzatore
}

Transformations::!Transform3D() {
    delete _transform; // Dealloca la memoria
}

void Transformations::Transform3D::SetPosition(float x, float y, float z) {
    _transform->SetPosition(x, y, z);
}

Vec3 Transformations::Transform3D::GetPosition() {
    return _transform->GetPosition();
}