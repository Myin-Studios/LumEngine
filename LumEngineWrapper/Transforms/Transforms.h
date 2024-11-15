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

#include "Transformations/Transformations.h"

#pragma make_public(Transform3DCore)

#include "../Math/Math.h"
#include "../Entities/Properties/Properties.h"

using namespace System;
using namespace System::Diagnostics;
using namespace LumScripting::Script::Properties;
using namespace System::Runtime::InteropServices;

public ref class Transform3D : public Property
{
private:
    Vec3Native position;
    Vec3Native rotation;
    Vec3Native scale;

public:
    Transform3D() : Property(new Transform3DCore())  // Passa Transform3DCore come IProperty
    {
        position = Vec3Native(0, 0, 0);
        rotation = Vec3Native(0, 0, 0);
        scale = Vec3Native(1, 1, 1);
    }

    property Vec3Native Position
    {
        Vec3Native get() { return position; }
        void set(Vec3Native value)
        {
            position = value;
            if (native)
            {
                static_cast<Transform3DCore*>(native)->SetPosition(value.x, value.y, value.z);
            }
        }
    }

    property Vec3Native Rotation
    {
        Vec3Native get() { return rotation; }
        void set(Vec3Native value)
        {
            rotation = value;
            if (native)
            {
                static_cast<Transform3DCore*>(native)->SetRotation(value.x, value.y, value.z);
            }
        }
    }

    property Vec3Native Scale
    {
        Vec3Native get() { return scale; }
        void set(Vec3Native value) { scale = value; }
    }
};