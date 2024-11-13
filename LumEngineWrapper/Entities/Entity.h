#pragma once

#include "../LumTypes/Entities/Entity.h"

namespace LumScripting
{
    namespace Script
    {
        namespace Entities
        {
            public ref class Entity {
            private:
                BaseEntity* native;

            public:
                Entity(BaseEntity* nativeEntity) : native(nativeEntity) {}
                Entity() : native(new BaseEntity()) {}

                // Metodo per aggiungere proprietà generiche
                template <typename T> void AddProperty(T prop) {
                    // Casting del valore passato a Object^
                    Object^ managedProperty = safe_cast<Object^>(prop);

                    // Converte Object^ in una forma utilizzabile per `BaseEntity`
                    // Ad esempio, potresti dover usare marshal_as o altre conversioni,
                    // a seconda di come funziona `BaseEntity`
                    native->AddProperty<T>(prop);  // Chiama il metodo nativo di BaseEntity
                }

                // Metodo per ottenere proprietà generiche
                template <typename T> T GetProperty() {
                    // Recupera la proprietà come `Object^` dal lato nativo
                    Object^ managedProperty = native->GetProperty();

                    // Esegue il cast a `T` e restituisce il valore
                    return safe_cast<T>(managedProperty);
                }
            };
        }
    }
}