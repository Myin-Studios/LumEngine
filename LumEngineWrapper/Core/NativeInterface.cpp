#include "NativeInterface.h"

RendererCore* Wrapper_GetRendererInstance() {
    return GetEngineRenderer();
}

BaseEntity* Wrapper_GetEntityAt(int index) {
    return GetEngineEntityAt(index);
}

int Wrapper_GetEntityCount() {
    return GetEngineEntityCount();
}