#pragma once

#include "../../Engine/Math/Transforms.h"

using namespace System;

public ref class Transform3DWrapper {
public:
    Transform3DWrapper() {
        _transform = new Transform3DCore();
    }

    ~Transform3DWrapper() {
        this->!Transform3DWrapper(); // Chiama il finalizzatore
    }

    !Transform3DWrapper() {
        delete _transform; // Dealloca la memoria
    }

    void SetPosition(float x, float y, float z) {
        _transform->SetPosition(x, y, z);
    }

    Vec3 GetPosition() {
        return _transform->GetPosition();
    }

private:
    Transform3DCore* _transform; // Puntatore alla tua classe originale
};