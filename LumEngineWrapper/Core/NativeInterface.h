#pragma once

#include "../LumTypes/Entities/Entity.h"
#include "../LumEngine/Engine/GUI/Rendering/renderer.h"

extern "C" {
    extern RendererCore* Wrapper_GetRendererInstance();
    extern BaseEntity* Wrapper_GetEntityAt(int index);
    extern int Wrapper_GetEntityCount();
}