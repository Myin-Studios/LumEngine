#include "Windows/mainwindow.h"
#include "Panels/panel.h"
#include "Console/console.h"
#include "Console/consoleOutput.h"
#include "Console/redirectStreamBuf.h"
#include "Panels/BaseProperties/baseProperties.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include "Panels/properties.h"
#include "Buttons/utilsbuttons.h"
#include "Buttons/tabsbutton.h"
#include "Rendering/renderer.h"
#include "GUIBuilder/IGUIBuilder.h"
#include <QPushButton>
#include <QSplitter>

#include <memory>

#pragma once

class GuiBuilder : public IGUIBuilder, public std::enable_shared_from_this<GuiBuilder>
{
public:
    GuiBuilder();
    ~GuiBuilder();

    void Initialize();
    void Cleanup();

    MainWindow* getMainWindow() const { return mainWindow; }
    PlayButton* getPlayButton() const { return playButton; }
    RendererCore* getScene() const { return scene; }

    virtual void SetEntity(BaseEntity* e) override;
    virtual BaseEntity* GetEntity() override;
	virtual void AddElement(const std::string& title, const std::string& propTitle) override;
	virtual void RemoveAllElements() override;

private:
    BaseEntity* _entity;

	RendererCore* scene;
    MainWindow* mainWindow;
    VPanel* topPanel;
    VerticalPanel* leftPanel;
    BasePanel* rightPanel;
    QSplitter* sceneConsoleSplitter;
    QSplitter* rightPanelSplitter;
    QWidget* mainContainer;
    TabsButton* tBtn;
    PlayButton* playButton;
    QVBoxLayout* centralLayout;
    BaseButton* lightingSettingsButton;
    BaseButton* hierarchyButton;
    Console* console;
};