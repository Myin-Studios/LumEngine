#pragma once

#include "Engine/GUI/Rendering/renderer.h"
#include "Engine/GUI/PropertyGroupManager/PropertyGroupManagerCore.h"

#ifdef LUMENGINE_EXPORTS
#define LUMENGINE_API __declspec(dllexport)
#else
#define LUMENGINE_API __declspec(dllimport)
#endif

class RendererCore;
class BaseEntity;

extern "C" {
    LUMENGINE_API RendererCore* GetEngineRenderer();
    LUMENGINE_API BaseEntity* GetEngineEntityAt(int index);
    LUMENGINE_API int GetEngineEntityCount();
}

class UIManager;