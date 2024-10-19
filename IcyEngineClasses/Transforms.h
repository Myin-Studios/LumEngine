// Transform3D.h
#pragma once

#include "Math.h"

using namespace System;

using namespace IcyScripting::Scripting::Mathematics;

namespace IcyScripting
{
    namespace Scripting
    {
        namespace Transforms
        {
            public ref class Transform3D
            {
            public:
                Transform3D() : position(gcnew Vec3()), rotation(gcnew Vec3()), scale(gcnew Vec3(1.0f, 1.0f, 1.0f)) {}

                // Proprietà per la posizione
                property Vec3^ Position {
                    Vec3^ get() { return position; }
                    void set(Vec3^ value) {
                        position = value;
                        // Emit a signal here if you're using a signaling mechanism
                    }
                }

                // Proprietà per la rotazione
                property Vec3^ Rotation {
                    Vec3^ get() { return rotation; }
                    void set(Vec3^ value) {
                        rotation = value;
                        // Emit a signal here if you're using a signaling mechanism
                    }
                }

                // Proprietà per la scala
                property Vec3^ Scale {
                    Vec3^ get() { return scale; }
                    void set(Vec3^ value) {
                        scale = value;
                        // Emit a signal here if you're using a signaling mechanism
                    }
                }

            private:
                Vec3^ position;
                Vec3^ rotation;
                Vec3^ scale;
            };
        }
    }
}

