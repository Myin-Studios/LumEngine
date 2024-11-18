#pragma once
#include "../LumTypes/Entities/Entity.h"
#include "Core/NativeInterface.h"

public ref class Renderer {
private:
    static Renderer^ instance;

    Renderer() { }

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
        return ::GetEntityAt(index);
    }

    int GetEntityCount() {
        return ::GetEntityCount();
    }
};