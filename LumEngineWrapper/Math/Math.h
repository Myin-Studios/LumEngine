/////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                   ///
///                                      THIS CODE IS PART OF:                                        ///
///                                       CryoMoon Engine (C)                                         ///
///                                                                                                   ///
///                                     WHICH IS LICENSED UNDER                                       ///
///                                          MIT License                                              ///
///                                      All Right Reserved.                                          ///
///                                                                                                   ///
/////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                   ///
///   Copyright (c) 2024 Myin Studios                                                                 ///
///                                                                                                   ///
///   Permission is hereby granted, free of charge,  to any person obtaining a copy of this software  ///
///   and associated documentation files (the "Software"), to deal in the Software without            ///
///   restriction, including without limitation the rights to use, copy, modify, merge, publish,      ///
///   distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the   ///
///   Software is furnished to do so, subject to the following conditions:                            ///
///                                                                                                   ///
///   The above copyright notice and this permission notice shall be included in all copies or        ///
///   substantial portions of the Software.                                                           ///
///                                                                                                   ///
///   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING   ///
///   BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND      ///
///   NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,    ///
///   DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  ///
///   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.         ///
///                                                                                                   ///
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Mathematics/Math.h"

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