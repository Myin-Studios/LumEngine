#pragma once

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