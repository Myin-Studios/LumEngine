#pragma once

class IGameBehaviour {
public:
    virtual void OnStart() = 0;
    virtual void OnRun() = 0;
    virtual ~IGameBehaviour() = default;
};