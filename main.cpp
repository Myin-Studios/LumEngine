#include "Engine/LogSystem/LogSystem.h"
#include "Math/CMMath.h"
#include "GameBehaviour/CMGameBehaviour.h"
#include <thread>
#include <chrono>

class MovingObject : public GameBehaviour
{
    CMVec3 v;
public:
    void onStart() override
    {
        CMLog::Debug("Hello from MovingObject!");
        v = CMVec3(2, 1, 5);

        //v.rotate(90.0, 0.0, 0.0);

        CMVec3 orth = CMVec3::orthogonalize(v);

        CMLog::Debug(orth);
        CMLog::Debug(CMVec3::dot(v, orth));
    }

    void onRun() override
    {

    }
};

class Ciao : public GameBehaviour
{
public:
    void onStart() override
    {
        CMLog::Debug("Hello from Ciao!");
    }

    void onRun() override {}
};

class GameCore : public CMTime
{
private:
    static std::chrono::steady_clock::time_point prevTime;

public:
    static void update()
    {
        std::chrono::steady_clock::time_point currTime = std::chrono::steady_clock::now();

        auto _dt = std::chrono::duration_cast<std::chrono::duration<double>>(currTime - prevTime);

        CMTime::_dt = _dt.count();

        prevTime = currTime;
    }
};

std::chrono::steady_clock::time_point GameCore::prevTime = std::chrono::steady_clock::now();

int main()
{
    MovingObject mo;
    Ciao c;

    mo.getClassName();
    c.getClassName();

    for (const auto& vec : *ScriptCore<GameBehaviour*>::gameScripts)
        vec->onStart();

    while (true)
    {
        //mo.onRun();

        GameCore::update();

        //CMLog::Debug(CMTime::dt());

        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 60));
    }


	return 0;
}