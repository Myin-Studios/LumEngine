#include "camera.h"

Transform3DCore* Camera::GetTransform() const { return this->transform; }

void Camera::SetTransform(Transform3DCore* t) { this->transform = t; }

