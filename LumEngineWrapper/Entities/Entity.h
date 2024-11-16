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
                EntityInternal(BaseEntity* nativeEntity) : native(nativeEntity)
                {
                    System::Console::WriteLine("1. EntityInternal costruito con native: " +
                        (native != nullptr ? "valido" : "null"));
                }

                EntityInternal() : native(new BaseEntity())
                {
                    System::Console::WriteLine("1. EntityInternal costruito con nuovo BaseEntity");
                }

                void AddPropertyInternal(IProperty* nativeProp)
                {
                    native->AddProperty(std::unique_ptr<IProperty>(nativeProp));
                }

                IProperty* GetPropertyInternal(const type_info& expectedType)
                {
                    return native->GetProperty(expectedType);
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