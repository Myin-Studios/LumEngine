#pragma once
#include "../LumTypes/Entities/Entity.h"
#include "Core/NativeInterface.h"

using namespace System;

public ref class Renderer {
private:
    static Renderer^ instance;
    IntPtr nativeRenderer;  // Handle per il RendererCore nativo

    Renderer() {
        // Ottieni l'istanza nativa e salvala
        nativeRenderer = IntPtr(Wrapper_GetRendererInstance());
    }

public:
    static property Renderer^ Instance {
        Renderer^ get() {
            if (instance == nullptr) {
                instance = gcnew Renderer();
            }
            return instance;
        }
    }

    BaseEntity* GetEntityAt(int index) {
        return Wrapper_GetEntityAt(index);
    }

    int GetEntityCount() {
        return Wrapper_GetEntityCount();
    }
};