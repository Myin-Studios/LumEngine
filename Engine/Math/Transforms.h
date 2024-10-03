/////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                   ///
///                                      THIS CODE IS PART OF:                                        ///
///                                       CryoMoon Engine (C)                                         ///
///                                                                                                   ///
///                                     WHICH IS LICENSED UNDER                                       ///
///                                          MIT License                                              ///
///                                      All Right Reserved.                                          ///
///                                                                                                   ///
/////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                   ///
///   Copyright (c) 2024 Myin Studios                                                                 ///
///                                                                                                   ///
///   Permission is hereby granted, free of charge,  to any person obtaining a copy of this software  ///
///   and associated documentation files (the "Software"), to deal in the Software without            ///
///   restriction, including without limitation the rights to use, copy, modify, merge, publish,      ///
///   distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the   ///
///   Software is furnished to do so, subject to the following conditions:                            ///
///                                                                                                   ///
///   The above copyright notice and this permission notice shall be included in all copies or        ///
///   substantial portions of the Software.                                                           ///
///                                                                                                   ///
///   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING   ///
///   BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND      ///
///   NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,    ///
///   DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  ///
///   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.         ///
///                                                                                                   ///
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Math.h"
#include "../Core/CMAPICore.h"
#include "glm/glm.hpp"
//#include "LogSystem.h"

class Transform3D
{
public:
    ~Transform3D() = default;

    Vec3 position;
    Vec3 rotation;
    Vec3 scale;

    Vec3 forward = Vec3(0, 0, 1);
    Vec3 right = Vec3(1, 0, 0);
    Vec3 up = Vec3(0, 1, 0);
    Vec3 backward = Vec3(0, 0, -1);
    Vec3 left = Vec3(-1, 0, 0);
    Vec3 down = Vec3(0, -1, 0);

    Transform3D()
    {
        position = Vec3(0, 0, 0);
        rotation = Vec3(0, 0, 0);
        scale = Vec3(1, 1, 1);
    }

    void SetPosition(float x, float y, float z) {
        position = position + Vec3{x, y, z};
    }

    Vec3 GetPosition() const {
        return position;
    }
};

extern "C" {
    CM_API inline Transform3D* CreateTransform3D() {
        return new Transform3D();
    }

    CM_API inline void DestroyTransform3D(Transform3D* instance) {
        delete instance;
    }

    CM_API inline void SetPosition(Transform3D* instance, float x, float y, float z) {
        if (instance != nullptr)
        {
            instance->SetPosition(x, y, z);
        }
    }

    CM_API inline void GetPosition(Transform3D* instance, float* x, float* y, float* z) {
        if (instance != nullptr) {
            *x = instance->position.x();
            *y = instance->position.y();
            *z = instance->position.z();
        }
    }
}

class Transform2D
{
public:
    Vec2 position;
    Vec2 rotation;
    Vec2 scale;

    Transform2D()
    {
        position = Vec2(0, 0);
        rotation = Vec2(0, 0);
        scale = Vec2(1, 1);
    }
};