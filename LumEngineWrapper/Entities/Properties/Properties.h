#pragma once

#include "../LumTypes/Entities/Properties/Property.h"

using namespace System;

namespace LumScripting
{
    namespace Script
    {
        namespace Properties
        {
			public ref class Property abstract
            {
            public:
                Property(IProperty* prop) : native(prop) {}
                Property() : native(new IProperty()) {}

                // Aggiungiamo un distruttore virtuale per la corretta pulizia
                virtual ~Property()
                {
                    this->!Property();
                }

                virtual void OnSerialize() = 0;
                virtual void OnDeserialize() = 0;
            protected:
                IProperty* native;

                !Property()
                {
                    if (native != nullptr)
                    {
                        delete native;
                        native = nullptr;
                    }
                }

            internal:
                IProperty* GetNativeProperty() { return native; }

                virtual const type_info& GetNativeTypeInfo() abstract;
                virtual Property^ CreateFromNative(IProperty* prop) abstract;
            };
        }
    }
}