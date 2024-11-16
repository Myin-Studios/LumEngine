#pragma once

#include "../LumTypes/Entities/Properties/Property.h"

namespace LumScripting
{
    namespace Script
    {
        namespace Properties
        {
			public ref class Property abstract
            {
            protected:  // Cambiato da private a protected per permettere l'accesso alle classi derivate
                IProperty* native;

            public:
                Property(IProperty* prop) : native(prop) {}
                Property() : native(new IProperty()) {}

				virtual const System::Type^ GetNativeType() = 0;

                // Aggiungiamo un distruttore virtuale per la corretta pulizia
                virtual ~Property()
                {
                    this->!Property();
                }

            protected:
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