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
#include "../LumEngineWrapper/Entities/Entity.h"
#include "../LumTypes/Entities/Entity.h"

using namespace System;
using namespace System::IO;
using namespace System::Text;
using namespace System::Diagnostics;
using namespace LumScripting::Script::Properties;
using namespace LumScripting::Script::Entities;
using namespace System::Runtime::InteropServices;

public ref class Transform3D : public Property
{
private:
    Vec3^ _position;
    Vec3^ _rotation;
    Vec3^ _scale;

public:
    Transform3D() : Property(new Transform3DCore())  // Passa Transform3DCore come IProperty
    {
        _position = gcnew Vec3(0, 0, 0);
        _rotation = gcnew Vec3(0, 0, 0);
        _scale = gcnew Vec3(1, 1, 1);
    }

    Transform3D(Transform3DCore* t) : Property(t)
	{
		_position = gcnew Vec3(t->position->x(), t->position->y(), t->position->z());
		_rotation = gcnew Vec3(t->rotation.x(), t->rotation.y(), t->rotation.z());
		_scale = gcnew Vec3(t->scale.x(), t->scale.y(), t->scale.z());
    }

    property Vec3^ Position
    {
        Vec3^ get() { return _position; }
        void set(Vec3^ value)
        {
            _position = value;
            if (native)
            {
                static_cast<Transform3DCore*>(native)->SetPosition(value->x, value->y, value->z);
            }
        }
    }

    property Vec3^ Rotation
    {
        Vec3^ get() { return _rotation; }
        void set(Vec3^ value)
        {
            _rotation = value;
            if (native)
            {
                static_cast<Transform3DCore*>(native)->SetRotation(value->x, value->y, value->z);
            }
        }
    }

    property Vec3^ Scale
    {
        Vec3^ get() { return _scale; }
        void set(Vec3^ value) { _scale = value; }
    }

    virtual void OnSerialize() override
    {
        if (native)
        {
            static_cast<Transform3DCore*>(native)->OnSerialize();
        }
    }

    virtual void OnDeserialize() override
    {
        if (native)
        {
            static_cast<Transform3DCore*>(native)->OnDeserialize();
        }
    }

internal:
	const type_info& GetNativeTypeInfo() override
	{
		return typeid(Transform3DCore);
	}

	virtual Property^ CreateFromNative(IProperty* prop) override
	{
		return gcnew Transform3D(static_cast<Transform3DCore*>(prop));
	}
};