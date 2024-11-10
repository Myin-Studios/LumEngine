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

#include <memory>

#pragma once

class GuiBuilder
{
public:
    GuiBuilder();
    ~GuiBuilder();

    MainWindow* getMainWindow() const { return mainWindow.get(); }
    PlayButton* getPlayButton() const { return playButton.get(); }

private:
    std::unique_ptr<MainWindow> mainWindow;

    std::unique_ptr<QHBoxLayout> positionLayout;
    std::unique_ptr<QHBoxLayout> topLine;
    std::unique_ptr<QVBoxLayout> centralLayout;

    std::unique_ptr<Renderer> scene;
    std::unique_ptr<Console> console;
    std::unique_ptr<RedirectStreamBuf> outputRedirector;

    std::unique_ptr<QSplitter> sceneConsoleSplitter;
    std::unique_ptr<QSplitter> rightPanelSplitter;
    std::unique_ptr<QWidget> mainContainer;

    std::unique_ptr<Panel> topPanel;
    std::unique_ptr<VerticalPanel> leftPanel;

    std::unique_ptr<BaseButton> consoleButton;
    std::unique_ptr<BaseButton> lightingSettingsButton;
    std::unique_ptr<BaseButton> hierarchyButton;

    std::unique_ptr<QLabel> positionLabel;
    std::unique_ptr<Vec3Property> position;

    std::unique_ptr<PlayButton> playButton;
    std::unique_ptr<TabsButton> tBtn;
};