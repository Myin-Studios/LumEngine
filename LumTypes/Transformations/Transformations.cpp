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

#include "Transformations.h"

Transform3DCore::Transform3DCore(const Transform3DCore& other)
    : position(new Vec3Core(*other.position))
    , rotation(other.rotation)
    , scale(other.scale)
    , forward(other.forward)
    , right(other.right)
    , up(other.up)
    , backward(other.backward)
    , left(other.left)
    , down(other.down)
    , _yaw(other._yaw)
    , _pitch(other._pitch)
    , _roll(other._roll)
{}

Transform3DCore& Transform3DCore::operator=(const Transform3DCore & other)
{
    if (this != &other) {
        delete position;
        position = new Vec3Core(*other.position);
        rotation = other.rotation;
        scale = other.scale;
        forward = other.forward;
        right = other.right;
        up = other.up;
        backward = other.backward;
        left = other.left;
        down = other.down;
        _yaw = other._yaw;
        _pitch = other._pitch;
        _roll = other._roll;
    }
    return *this;
}

void Transform3DCore::Move(float x, float y, float z)
{
    Vec3Core* tmp = new Vec3Core(x, y, z);
    this->position->operator+=(*tmp);
    delete tmp;  // Dealloca la memoria
}

void Transform3DCore::Move(Vec3Core* v)
{
    this->position->operator+=(*v);
}

void Transform3DCore::SetRotation(float yaw, float pitch, float roll)
{
    this->_yaw = yaw;
    this->_pitch = pitch;
    this->_roll = roll;
}

void Transform3DCore::Rotate(float yaw, float pitch, float roll)
{
    this->_yaw += yaw;
    this->_pitch += pitch;
    this->_roll += roll;

    // Limit pitch to avoid gimbal lock
    if (this->_pitch > 89.0f) this->_pitch = 89.0f;
    if (this->_pitch < -89.0f) this->_pitch = -89.0f;

    double PI = 3.14159265359;

    // Calculate front vector based on yaw and pitch
    Vec3Core front;
    front.setX(cos(this->_yaw * PI / 180) * cos(this->_pitch * PI / 180));
    front.setY(sin(this->_pitch * PI / 180));
    front.setZ(sin(this->_yaw * PI / 180) * cos(this->_pitch * PI / 180));

    // Set forward direction
    this->forward = front;

    // Calculate right and up vectors based on the forward vector
    this->right = Vec3Core::Cross(Vec3Core(0.0f, -1.0f, 0.0f), this->forward).Normalize();
    this->up = Vec3Core::Cross(this->forward, this->right).Normalize();
}
