/////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                   ///
///                                      THIS CODE IS PART OF:                                        ///
///                                          LumEngine (C)                                            ///
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

#include "../Mathematics/Math.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <filesystem>

#include "../Entities/Properties/Property.h"

class Transform3DCore : public IProperty
{
public:
    ~Transform3DCore() {
        // std::cout << "Transform3DCore instance destroyed." << std::endl;

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

    Transform3DCore(const Transform3DCore& other);

    Transform3DCore& operator=(const Transform3DCore& other);

    /* --------------------------{ Position }--------------------------*/

    void SetPosition(float x, float y, float z) {
        position->setX(x);
        position->setY(y);
        position->setZ(z);
    }

    void Move(float x, float y, float z);

    void Move(Vec3Core* v);

    Vec3Core& GetPosition() const {
        return *position;
    }

    /* --------------------------{ Rotation }--------------------------*/

    void SetRotation(float yaw, float pitch, float roll);
    void Rotate(float yaw, float pitch, float roll);

    /* --------------------------{ Serialization }--------------------------*/

    void OnSerialize() override
    {
        if (!std::filesystem::exists("SerializedData")) {
            std::filesystem::create_directory("SerializedData");
        }

        std::ofstream file("SerializedData/transform3d.bin", std::ios::binary);

        if (!file.is_open())
        {
            std::cerr << "Error during file opening!" << std::endl;
            return;
        }

        if (file.good())
        {
            file.write(reinterpret_cast<char*>(position), sizeof(Vec3Core));
            file.write(reinterpret_cast<char*>(&rotation), sizeof(Vec3Core));
            file.write(reinterpret_cast<char*>(&scale), sizeof(Vec3Core));
        }

        file.close();
    }

    void OnDeserialize() override
    {
        std::ifstream file("SerializedData/transform3d.bin", std::ios::binary);

        if (!file.is_open())
        {
            std::cerr << "Error during file opening!" << std::endl;
            return;
        }

        if (file.good())
        {
            file.read(reinterpret_cast<char*>(position), sizeof(Vec3Core));
            file.read(reinterpret_cast<char*>(&rotation), sizeof(Vec3Core));
            file.read(reinterpret_cast<char*>(&scale), sizeof(Vec3Core));
        }

        file.close();
    }

private:
    float _yaw = 0.0f;
    float _pitch = 0.0f;
    float _roll = 0.0f;
};