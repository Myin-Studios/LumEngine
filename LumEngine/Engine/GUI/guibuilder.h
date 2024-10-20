#include "Windows/mainwindow.h"
#include "Panels/panel.h"
#include "Console/console.h"
#include "Console/consoleOutput.h"
#include "Console/redirectStreamBuf.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include "Panels/properties.h"
#include "Buttons/utilsbuttons.h"
#include "Buttons/tabsbutton.h"
#include "Rendering/renderer.h"
#include <QPushButton>
#include <QSplitter>

#pragma once

class GuiBuilder
{
public:
    GuiBuilder();
    ~GuiBuilder();

    MainWindow* getMainWindow() const { return mainWindow; }
    PlayButton* getPlayButton() const { return playButton; }
private:
    MainWindow* mainWindow = nullptr;

    QHBoxLayout* positionLayout = nullptr;
    QHBoxLayout* topLine = nullptr;
    QVBoxLayout* centralLayout = nullptr;

    Renderer* scene = nullptr;
    Console* console = nullptr;
    RedirectStreamBuf* outputRedirector = nullptr;

    QSplitter* sceneConsoleSplitter = nullptr;

    Panel* topPanel = nullptr;
    Panel* rightPanel = nullptr;

    QLabel* positionLabel = nullptr;
    Vec3Property* position = nullptr;

    PlayButton* playButton = nullptr;
    TabsButton* tBtn = nullptr;
};