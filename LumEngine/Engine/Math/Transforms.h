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
#include <cmath>

class Transform3DCore
{
public:
    ~Transform3DCore() {
        std::cout << "Transform3DCore instance destroyed." << std::endl;

        delete position;
        position = nullptr;
    }

    Vec3Core* position;
    Vec3Core rotation;
    Vec3Core scale;

    Vec3Core forward = Vec3Core(0, 0, 1);
    Vec3Core right = Vec3Core(1, 0, 0);
    Vec3Core up = Vec3Core(0, 1, 0);
    Vec3Core backward = Vec3Core(0, 0, -1);
    Vec3Core left = Vec3Core(-1, 0, 0);
    Vec3Core down = Vec3Core(0, -1, 0);

    Transform3DCore()
        : position(new Vec3Core(0, 0, 0)),
        rotation(0, 0, 0),
        scale(1, 1, 1)
    {}

    /* --------------------------{ Position }--------------------------*/

    void SetPosition(float x, float y, float z) {
        position = new Vec3Core(x, y, z);
        std::cout << "Position updated to: " << position->x() << ", " << position->y() << ", " << position->z() << std::endl;
    }

    void Move(float x, float y, float z);
    
    void Move(Vec3Core* v);

    Vec3Core& GetPosition() const {
        return *position;
    }

    /* --------------------------{ Rotation }--------------------------*/

    void SetRotation(float yaw, float pitch, float roll);
    void Rotate(float yaw, float pitch, float roll);

private:
    float _yaw = 0.0f;
    float _pitch = 0.0f;
    float _roll = 0.0f;
};

/*
extern "C" {
    CM_API inline Transform3DCore* CreateTransform3D() {
        return new Transform3DCore();
    }

    CM_API inline void DestroyTransform3D(Transform3DCore* instance) {
        delete instance;
    }

    CM_API inline void SetPosition(Transform3DCore* instance, float x, float y, float z) {
        if (instance != nullptr)
        {
            std::cout << "Setting position to: " << x << ", " << y << ", " << z << std::endl;
            instance->SetPosition(x, y, z);
        }
        else cerr << "Instance is null!" << endl;
    }

    CM_API inline void GetPosition(Transform3DCore* instance, float* x, float* y, float* z) {
        if (instance != nullptr) {
            *x = instance->position->x();
            *y = instance->position->y();
            *z = instance->position->z();
        }
    }
}
*/

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