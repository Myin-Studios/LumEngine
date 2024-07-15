/////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                   ///
///                                      THIS CODE IS PART OF:                                        ///
///                                       CryoMoon Engine (C)                                         ///
///                                                                                                   ///
///                                     WHICH IS LICENSED UNDER                                       ///
///                                          MIT License                                              ///
///                                      All Right Reserved.                                          ///
///                                                                                                   ///
/////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                   ///
///   Copyright (c) 2024 Myin Studios                                                                 ///
///                                                                                                   ///
///   Permission is hereby granted, free of charge,  to any person obtaining a copy of this software  ///
///   and associated documentation files (the "Software"), to deal in the Software without            ///
///   restriction, including without limitation the rights to use, copy, modify, merge, publish,      ///
///   distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the   ///
///   Software is furnished to do so, subject to the following conditions:                            ///
///                                                                                                   ///
///   The above copyright notice and this permission notice shall be included in all copies or        ///
///   substantial portions of the Software.                                                           ///
///                                                                                                   ///
///   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING   ///
///   BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND      ///
///   NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,    ///
///   DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  ///
///   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.         ///
///                                                                                                   ///
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Engine/LogSystem/LogSystem.h"
#include "GameBehaviour/CMGameBehaviour.h"
#include <thread>
#include <chrono>
#include "Transforms.h"
#include "GameBehaviour/CMTime.h"
#include "GUI/MainWindow/renderer.h"

class Ciao : public BaseProperty
{
public:
};

class BaseScript : public GameBehaviour
{
    PropertyManager b{};

public:
    Transform3D t;
    Transform2D t2;

    //called on startup...
    void onStart() override
    {
        t2.position = Vec2(2, 54);
        Vec2 p = t2.position.normalize();

        b.addProperty<Transform3D>();
        b.addProperty<Ciao>();

        b.getProperty<Transform3D>()->position = Vec3(23, 44, 50);
        Vec3 pos = b.getProperty<Transform3D>()->position;

        b.getPropertyList();
        Log::Debug(p);
    }

    //called once per frame...
    void onRun() override
    {

    }
};

class SecondScript : public GameBehaviour
{
    PropertyManager b{};

    void onStart() override
    {
        b.addProperty<Ciao>();
        b.addProperty<Transform3D>();

        b.getPropertyList();
    }

    void onRun() override
    {

    }
};

int main(int argc, char ** argv)
{
//    MainWindow engineMainWin(800, 600, "CryoMoon Engine");
//    engineMainWin.Run();

//    QApplication app(argc, argv);
//
//    MainWindow mw;
//
//    app.setStyle("Fusion");
//
//    mw.init();
//
//    app.setWindowIcon(QIcon(":/Assets/CryoMoonLogo.ico"));
//
//    mw.show();
//
//    return app.exec();

    wxApp::SetInstance(new App());
    wxEntry(argc, argv);

    return 0;

    BaseScript mo;
    SecondScript sc;
    Ciao c;

    for (const auto& vec : *ScriptCore<GameBehaviour*>::gameScripts)
        vec->onStart();

    while (true)
    {
        mo.onRun();

        GameCore::update();

        //Log::Debug(CMTime::dt());

        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 60));
    }
}