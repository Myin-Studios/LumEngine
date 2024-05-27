#include <iostream>
#include "CMTime.h"
#include <vector>
#include "Core/CMCore.h"

using namespace std;

class GameBehaviour : private ScriptCore<GameBehaviour*> {
private:
    GameBehaviour* inst = nullptr;

public:
    GameBehaviour()
    {
        if (ScriptCore::gameScripts == nullptr)
            gameScripts = new vector<GameBehaviour*>();

        inst = this;
        gameScripts->push_back(inst);
    }

    void getClassName() const { CMLog::Debug(typeid(*this).name()); }

    virtual void onStart() = 0;
    virtual void onRun() = 0;
};

//vector<GameBehaviour*>* GameBehaviour::gameScripts = nullptr;