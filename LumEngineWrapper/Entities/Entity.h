#pragma once

#include "../LumTypes/Entities/Entity.h"
#include "Properties/Properties.h"

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

                IProperty* GetPropertyInternal()
                {
                    return native->GetProperty<IProperty>();
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

                    // Convertiamo la proprietà managed in nativa
                    System::IntPtr ptr = System::Runtime::InteropServices::Marshal::AllocHGlobal(sizeof(T));
                    System::Runtime::InteropServices::Marshal::StructureToPtr(prop, ptr, false);
                    IProperty* nativeProp = static_cast<IProperty*>(ptr.ToPointer());

                    internal->AddPropertyInternal(nativeProp);
                }

                generic<typename T>
                where T : LumScripting::Script::Properties::Property
                T GetProperty()
                {
                    IProperty* nativeProp = internal->GetPropertyInternal();
                    if (nativeProp == nullptr)
                        return {};

                    return safe_cast<T>(System::Runtime::InteropServices::Marshal::PtrToStructure(
                        System::IntPtr(nativeProp),
                        T::typeid));
                }
            };
        }
    }
}