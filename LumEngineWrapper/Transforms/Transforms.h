#pragma once

#include "../../../LumEngine/LumEngine/Engine/Math/Transforms.h"
#include "../Math/Math.h"

using namespace LumScripting::Script::Math;

namespace LumScripting
{
    namespace Script
    {
        namespace Transformations
        {
            public ref class Transform3D
            {
            private:
                Transform3DCore* coreTransform; // Puntatore alla classe nativa

            public:
                Transform3D()
                {
                    coreTransform = new Transform3DCore(); // Allocazione nativa
                }

                ~Transform3D() {
                    this->!Transform3D(); // Distruzione manuale
                }

                !Transform3D() {
                    if (coreTransform != nullptr) {
                        delete coreTransform;
                        coreTransform = nullptr;
                    }
                }

                void SetPosition(float x, float y, float z) {
                    Console::WriteLine(String::Format("Position updated to: ({0}, {1}, {2})", x, y, z));
                    coreTransform->SetPosition(x, y, z);
                }

                Vec3^ GetPosition() {
                    return gcnew Vec3(coreTransform->GetPosition().x(), coreTransform->GetPosition().y(), coreTransform->GetPosition().z()); // Conversione da classe nativa a gestita
                }
            };
        }
    }
}