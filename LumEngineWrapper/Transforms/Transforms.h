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

#include "../LumTypes/Transformations/Transformations.h"
#include "../Math/Math.h"

using namespace System;
using namespace System::Diagnostics;

public ref class Transform3D
{
private:
    Transform3DCore* coreTransform;

public:
    Transform3D()
    {
        Console::WriteLine("=== Transform3D Initialization Debug ===");
        Console::WriteLine("AppDomain: " + AppDomain::CurrentDomain->FriendlyName);
        Console::WriteLine("Base Directory: " + AppDomain::CurrentDomain->BaseDirectory);
        Console::WriteLine("Current Process: " + Process::GetCurrentProcess()->ProcessName);

        try
        {
            Console::WriteLine("Creating Transform3DCore...");
            coreTransform = new Transform3DCore();
            Console::WriteLine("Transform3DCore created successfully");

            // Test immediato del coreTransform
            coreTransform->SetPosition(0, 0, 0);
            Console::WriteLine("Initial position set successfully");
        }
        catch (System::Exception^ e)
        {
            Console::WriteLine("Managed Exception: " + e->Message);
            Console::WriteLine("Stack: " + e->StackTrace);
            throw;
        }
        catch (const std::exception& e)
        {
            Console::WriteLine("Native Exception: " + gcnew String(e.what()));
            throw;
        }
        catch (...)
        {
            Console::WriteLine("Unknown Exception during initialization");
            throw;
        }
    }

    ~Transform3D()
    {
        Console::WriteLine("Transform3D destructor - start");
        if (coreTransform != nullptr)
        {
            delete coreTransform;
            coreTransform = nullptr;
        }
        Console::WriteLine("Transform3D destructor - end");
    }

    void SetPosition(float x, float y, float z)
    {
        Console::WriteLine(String::Format("SetPosition called with: ({0}, {1}, {2})", x, y, z));
        if (coreTransform == nullptr)
        {
            Console::WriteLine("Error: coreTransform is null in SetPosition");
            return;
        }
        try
        {
            coreTransform->SetPosition(x, y, z);
            Console::WriteLine("SetPosition completed successfully");
        }
        catch (const std::exception& e)
        {
            Console::WriteLine("Exception in SetPosition: " + gcnew String(e.what()));
            throw;
        }
    }

    Vec3^ GetPosition()
    {
        Console::WriteLine("GetPosition called");
        if (coreTransform == nullptr)
        {
            Console::WriteLine("Error: coreTransform is null in GetPosition");
            return nullptr;
        }
        try
        {
            Vec3Core& pos = coreTransform->GetPosition();
            return gcnew Vec3(pos.x(), pos.y(), pos.z());
        }
        catch (const std::exception& e)
        {
            Console::WriteLine("Exception in GetPosition: " + gcnew String(e.what()));
            throw;
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