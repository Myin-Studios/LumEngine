#pragma once

#include "../LumTypes/Entities/Entity.h"
#include "../LumEngine/Engine/GUI/Rendering/renderer.h"

extern "C" {
    __declspec(dllexport) BaseEntity* GetEntityAt(int index)
    {
        RendererCore* renderer = RendererCore::GetInstance();
        if (renderer && index >= 0 && index < renderer->GetEntities().size()) {
            return renderer->GetEntities()[index].get();
        }
        return nullptr;
    }
    __declspec(dllexport) int GetEntityCount()
    {
        RendererCore* renderer = RendererCore::GetInstance();
        return renderer ? renderer->GetEntities().size() : 0;
    }
}