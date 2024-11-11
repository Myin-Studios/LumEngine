#pragma once

#include "../LumTypes/Transformations/Transformations.h"
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
                    try
                    {
                        coreTransform = new Transform3DCore(); // Allocazione nativa
                    }
                    catch (const std::exception& e)
                    {
                        Console::WriteLine("Exception during Transform3DCore initialization: " + gcnew String(e.what()));
                        coreTransform = nullptr;
                    }
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
                    if (coreTransform == nullptr) {
                        Console::WriteLine("coreTransform is null in SetPosition.");
                        return;
                    }
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