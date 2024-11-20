#pragma once

#include "../LumTypes/Entities/Properties/Property.h"
#include <vcclr.h>

using namespace System;

namespace LumScripting
{
    namespace Script
    {
        namespace Properties
        {
            ref class Property;

            class BridgeProperty : public IProperty {
            private:
                gcroot<Property^> managed;  // Reference all'oggetto managed

            public:
                BridgeProperty(Property^ owner) : managed(owner) {}

                virtual void OnSerialize() override;

                virtual void OnDeserialize() override;
            };

            public ref class Property abstract
            {
            public:
                Property(IProperty* prop) : native(prop) {}
                Property() : native(new BridgeProperty(this)) {}  // Usa DefaultProperty invece di IProperty

                virtual ~Property()
                {
                    this->!Property();
                }

                virtual void OnSerialize() abstract;
                virtual void OnDeserialize() abstract;
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

                virtual const type_info& GetNativeTypeInfo() { return typeid(IProperty); }
                virtual Property^ CreateFromNative(IProperty* prop) { return this; }
            };

            void BridgeProperty::OnSerialize() {
                managed->OnSerialize();
            }

            void BridgeProperty::OnDeserialize() {
                managed->OnDeserialize();
            }
        }
    }
}