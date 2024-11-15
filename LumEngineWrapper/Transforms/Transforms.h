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

#include "Transformations/Transformations.h"
#include "../Math/Math.h"
#include "../Entities/Properties/Properties.h"

using namespace System;
using namespace System::Diagnostics;
using namespace LumScripting::Script::Properties;

public ref class Transform3D : public Property
{
private:
    Transform3DCore* coreTransform;
    bool disposed;

public:
    Transform3D()
    {
        Console::WriteLine("=== Transform3D Initialization Debug ===");
        disposed = false;
        try
        {
            Console::WriteLine("Creating Transform3DCore...");
            coreTransform = new Transform3DCore();
            Console::WriteLine("Transform3DCore created successfully");
        }
        catch (System::Exception^ e)
        {
            Console::WriteLine("Managed Exception: " + e->Message);
            throw;
        }
    }

    // Distruttore - cleanup managed e unmanaged
    ~Transform3D()
    {
        if (!disposed)
        {
            // Chiamata al finalizer per il cleanup unmanaged
            this->!Transform3D();
            disposed = true;
        }
        // Sopprime la finalizzazione poiché abbiamo già fatto il cleanup
        System::GC::SuppressFinalize(this);
    }

    // Finalizer - cleanup solo unmanaged
    !Transform3D()
    {
        if (!disposed)
        {
            if (coreTransform != nullptr)
            {
                delete coreTransform;
                coreTransform = nullptr;
            }
            disposed = true;
        }
    }

    void SetPosition(float x, float y, float z)
    {
        if (disposed)
        {
            throw gcnew System::ObjectDisposedException("Transform3D");
        }

        Console::WriteLine(String::Format("SetPosition called with: ({0}, {1}, {2})", x, y, z));
        if (coreTransform == nullptr)
        {
            throw gcnew System::NullReferenceException("coreTransform is null");
        }

        try
        {
            coreTransform->SetPosition(x, y, z);
            Console::WriteLine("SetPosition completed successfully");
        }
        catch (const std::exception& e)
        {
            throw gcnew System::Exception(gcnew String(e.what()));
        }
    }

    Vec3^ GetPosition()
    {
        if (disposed)
        {
            throw gcnew System::ObjectDisposedException("Transform3D");
        }

        Console::WriteLine("GetPosition called");
        if (coreTransform == nullptr)
        {
            throw gcnew System::NullReferenceException("coreTransform is null");
        }

        try
        {
            Vec3Core& pos = coreTransform->GetPosition();
            return gcnew Vec3(pos.x(), pos.y(), pos.z());
        }
        catch (const std::exception& e)
        {
            throw gcnew System::Exception(gcnew String(e.what()));
        }
    }
};

// using namespace LumScripting::Script::Math;

// namespace LumScripting
// {
//     namespace Script
//     {
//         namespace Transformations
//         {
//             
// 
//             // public ref class Transform3D
//             // {
//             // private:
//             //     Transform3DCore* coreTransform; // Puntatore alla classe nativa
//             // 
//             // public:
//             //     Transform3D()
//             //     {
//             //         Console::WriteLine("Tranform3D constructor.");
//             // 
//             //         // try
//             //         // {
//             //         //     coreTransform = new Transform3DCore(); // Allocazione nativa
//             //         // }
//             //         // catch (const std::exception& e)
//             //         // {
//             //         //     Console::WriteLine("Exception during Transform3DCore initialization: " + gcnew String(e.what()));
//             //         //     coreTransform = nullptr;
//             //         // }
//             //     }
//             // 
//             //     Transform3D(Transform3D^ other) {
//             //         try {
//             //             if (other->coreTransform != nullptr) {
//             //                 coreTransform = new Transform3DCore(*other->coreTransform);
//             //             }
//             //             else {
//             //                 coreTransform = new Transform3DCore();
//             //             }
//             //         }
//             //         catch (const std::exception& e) {
//             //             Console::WriteLine("Exception in copy constructor: " + gcnew String(e.what()));
//             //             coreTransform = nullptr;
//             //         }
//             //     }
//             // 
//             //     ~Transform3D() {
//             //         this->!Transform3D(); // Distruzione manuale
//             //     }
//             // 
//             //     !Transform3D() {
//             //         try {
//             //             if (coreTransform != nullptr) {
//             //                 delete coreTransform;
//             //                 coreTransform = nullptr;
//             //             }
//             //         }
//             //         catch (const std::exception& e) {
//             //             Console::WriteLine("Exception in finalizer: " + gcnew String(e.what()));
//             //         }
//             //     }
//             // 
//             //     void SetPosition(float x, float y, float z) {
//             //         if (coreTransform == nullptr) {
//             //             Console::WriteLine("coreTransform is null in SetPosition.");
//             //             return;
//             //         }
//             //         Console::WriteLine(String::Format("Position updated to: ({0}, {1}, {2})", x, y, z));
//             //         coreTransform->SetPosition(x, y, z);
//             //     }
//             // 
//             //     Vec3^ GetPosition() {
//             //         try {
//             //             if (coreTransform == nullptr) {
//             //                 Console::WriteLine("coreTransform is null in GetPosition.");
//             //                 return nullptr;
//             //             }
//             //             return gcnew Vec3(coreTransform->GetPosition().x(),
//             //                 coreTransform->GetPosition().y(),
//             //                 coreTransform->GetPosition().z());
//             //         }
//             //         catch (const std::exception& e) {
//             //             Console::WriteLine("Exception in GetPosition: " + gcnew String(e.what()));
//             //             return nullptr;
//             //         }
//             //     }
//             // 
//             //     Transform3D^ operator=(Transform3D^ other)
//             //     {
//             //         if (this != other)
//             //         {
//             //             try {
//             //                 if (coreTransform != nullptr) {
//             //                     delete coreTransform;
//             //                     coreTransform = nullptr;
//             //                 }
//             // 
//             //                 if (other->coreTransform != nullptr) {
//             //                     coreTransform = new Transform3DCore(*other->coreTransform);
//             //                 }
//             //             }
//             //             catch (const std::exception& e) {
//             //                 Console::WriteLine("Exception in assignment operator: " + gcnew String(e.what()));
//             //                 coreTransform = nullptr;
//             //             }
//             //         }
//             //         return this;
//             //     }
//             // };
//         }
//     }
// }