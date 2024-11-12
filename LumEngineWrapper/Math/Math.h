#pragma once

#include "../LumTypes/Mathematics/Math.h"

using namespace System;

public ref class Vec3
{
public:
    float x, y, z;

    Vec3()
        : x(0), y(0), z(0) // Inizializza x, y, z a zero
    {
        vec3 = new Vec3Core();  // Crea un'istanza nativa
    }

    Vec3(float x, float y, float z) : x(x), y(y), z(z) // Costruttore con tre argomenti
    {
        vec3 = new Vec3Core(x, y, z); // Crea un'istanza nativa
    }

    ~Vec3()
    {
        this->!Vec3();  // Chiama il finalizzatore per liberare la memoria
    }

    !Vec3()  // Finalizzatore (destructor deterministico)
    {
        if (vec3 != nullptr)
        {
            delete vec3;  // Libera la memoria dell'oggetto nativo
            vec3 = nullptr; // Imposta a nullptr dopo la liberazione
        }
    }

    // Eventuali metodi aggiuntivi per gestire Vec3Core possono essere aggiunti qui

private:
    Vec3Core* vec3;  // Puntatore alla classe nativa
};

// namespace LumScripting
// {
//     namespace Script
//     {
//         namespace Math
//         {
//             
//         }
//     }
// }
// 