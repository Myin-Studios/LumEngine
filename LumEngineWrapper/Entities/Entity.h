#pragma once

#include "../LumTypes/Entities/Entity.h"
#include "Properties/Properties.h"

using namespace LumScripting::Script::Properties;

namespace LumScripting
{
    namespace Script
    {
        namespace Entities
        {
            private ref class EntityInternal
            {
            private:
                BaseEntity* native;

            public:
                EntityInternal(BaseEntity* nativeEntity) : native(nativeEntity) {}
                EntityInternal() : native(new BaseEntity()) {}

                void AddPropertyInternal(IProperty* nativeProp)
                {
                    native->AddProperty(std::unique_ptr<IProperty>(nativeProp));
                }

                IProperty* GetPropertyInternal(const type_info& expectedType)
                {
                    IProperty* prop = native->GetProperty<IProperty>();
                    if (prop && typeid(*prop) == expectedType) {
                        return prop;
                    }
                    return nullptr;
                }
            };

            public ref class Entity {
            private:
                EntityInternal^ internal;

            public:
                Entity() : internal(gcnew EntityInternal()) {}

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