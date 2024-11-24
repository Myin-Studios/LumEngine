#pragma once

#include "../LumTypes/Entities/Entity.h"
#include "Properties/Properties.h"
#include "../LumEngineWrapper/Rendering/Renderer.h"
#include "../LumTypes/GameBehaviour/GameBehaviour.h"

using namespace LumScripting::Script::Properties;

namespace LumScripting
{
    namespace Script
    {
        namespace Entities
        {
            public ref class NativeEntityFactory {
            public:
                static IntPtr CreateNativeEntity() {
                    BaseEntity* entity = new BaseEntity();
                    return IntPtr(entity);
                }

                static IntPtr GetExistingEntity(int index) {
                    BaseEntity* entity = Renderer::Instance->GetEntityAt(index);
                    return IntPtr(entity);
                }

                static int GetEntityCount() {
                    return Renderer::Instance->GetEntityCount();
                }
            };

            private ref class EntityInternal
            {
            private:
                BaseEntity* native;

            public:
                EntityInternal(BaseEntity* existingEntity) : native(existingEntity) {
                    System::Console::WriteLine("EntityInternal costruito con entity esistente");
                }

                void AddPropertyInternal(IProperty* nativeProp)
                {
                    native->AddProperty(std::unique_ptr<IProperty>(nativeProp));
                }

                IProperty* GetPropertyInternal(const type_info& expectedType)
                {
                    return native->GetProperty(expectedType);
                }

                void AddScriptInternal(IGameBehaviour* script)
                {
                    native->AddScript(std::unique_ptr<IGameBehaviour>(script));
                }

                void RemoveScriptInternal(IGameBehaviour* script)
                {
                    native->RemoveScript(script);
                }
            };

            public ref class Entity {
            private:
                EntityInternal^ internal;

            public:
                Entity(IntPtr nativePtr) {
                    internal = gcnew EntityInternal(static_cast<BaseEntity*>(nativePtr.ToPointer()));
                }

                generic<typename T>
                where T : LumScripting::Script::Properties::Property
                void AddProperty(T prop)
                {
                    if (prop == nullptr)
                        throw gcnew System::ArgumentNullException("prop");

                    // Assumendo che Property abbia un campo/metodo per accedere al puntatore nativo
                    IProperty* nativeProp = prop->GetNativeProperty(); // o accesso diretto al campo native
                    internal->AddPropertyInternal(nativeProp);
                }

                generic<typename T>
                where T : Property
                T GetProperty()
                {
                    // Creiamo direttamente l'istanza usando Activator
					System::Console::WriteLine("1. Chiamata GetProperty nel wrapper Entity");
                    Property^ temp = Activator::CreateInstance<T>();

                    const type_info& expectedType = temp->GetNativeTypeInfo();

                    IProperty* nativeProp = internal->GetPropertyInternal(expectedType);
                    if (nativeProp == nullptr)
                        return T();

                    return safe_cast<T>(temp->CreateFromNative(nativeProp));
                }
            };
        }
    }
}