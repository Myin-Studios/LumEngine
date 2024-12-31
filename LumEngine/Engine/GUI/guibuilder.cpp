#include "guibuilder.h"

GuiBuilder::GuiBuilder()
{
    mainWindow = new MainWindow();
    topPanel = new VPanel(mainWindow->centralWidget());
    leftPanel = new VerticalPanel(mainWindow->centralWidget());
    rightPanel = new BasePanel();
    sceneConsoleSplitter = new QSplitter(mainWindow->centralWidget());
    rightPanelSplitter = new QSplitter(mainWindow->centralWidget());
    mainContainer = new QWidget(mainWindow->centralWidget());
    tBtn = new TabsButton(mainWindow->centralWidget());
    playButton = new PlayButton(mainWindow->centralWidget());
    centralLayout = new QVBoxLayout(mainContainer);
    console = new Console(mainWindow->centralWidget());
	scene = RendererCore::GetInstance(mainWindow->centralWidget());
    lightingSettingsButton = new BaseButton();
    hierarchyButton = new BaseButton();
    
    mainWindow->setStyleSheet("background-color: rgb(20, 20, 20);");

    auto configureButton = [](QPushButton* button) {
        button->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
        button->setMinimumSize(0, button->sizeHint().height());
        };

    lightingSettingsButton->setText("");
    lightingSettingsButton->setIcon(QIcon("Resources/Icons/New/LumE_LightSettingsHover.svg"));
    lightingSettingsButton->setIconSize(QSize(20, 20));
    lightingSettingsButton->setCursor(Qt::PointingHandCursor);

    hierarchyButton->setText("");
    hierarchyButton->setIcon(QIcon("Resources/Icons/New/LumE_HierarchyHover.svg"));
    hierarchyButton->setIconSize(QSize(20, 20));
    hierarchyButton->setCursor(Qt::PointingHandCursor);

    configureButton(lightingSettingsButton);
    configureButton(hierarchyButton);

    playButton->setIconSize(QSize(20, 20));
    tBtn->setIconSize(QSize(20, 20));

    scene->setStyleSheet("border-radius: 30px");
    scene->setGeometry(scene->x(), scene->y(), 800, 600);
    scene->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    console->setStyleSheet("background-color: rgb(25, 25, 25);");

    // Setting window properties
    mainWindow->setGeometry(mainWindow->x(), mainWindow->y(), 1024, 768);
    mainWindow->setWindowIcon(QIcon(":/Icons/CMEngine_Logo.svg"));

    leftPanel->setStyleSheet("background-color: rgb(25, 25, 25);" "border-radius: 10px");
    leftPanel->setFixedWidth(40);
    leftPanel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    leftPanel->getLayout()->addWidget(tBtn);
    leftPanel->getLayout()->addWidget(lightingSettingsButton, 0, Qt::AlignCenter);
    leftPanel->getLayout()->addWidget(hierarchyButton);

    rightPanel->addHeader("PROPERTIES");
    topPanel->setStyleSheet("background-color: rgb(25, 25, 25);" "border-radius: 10px");
    topPanel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    topPanel->getLayout()->addWidget(playButton, 0, Qt::AlignLeft);

    sceneConsoleSplitter->addWidget(scene);
    sceneConsoleSplitter->addWidget(console);
    sceneConsoleSplitter->setOrientation(Qt::Vertical);

    centralLayout->addWidget(topPanel, 0, Qt::AlignTop);
    centralLayout->addWidget(sceneConsoleSplitter);

    centralLayout->setContentsMargins(0, 0, 0, 0);

    mainContainer->setLayout(centralLayout);

    rightPanelSplitter->addWidget(leftPanel);
    rightPanelSplitter->addWidget(mainContainer);
    rightPanelSplitter->addWidget(rightPanel);

    mainWindow->getMainLayout()->addWidget(rightPanelSplitter);

    mainWindow->show();
    leftPanel->show();

    // std::cout.rdbuf(outputRedirector.get());
    // std::cerr.rdbuf(outputRedirector.get());
}

GuiBuilder::~GuiBuilder()
{
    std::cout << "Destroying GuiBuilder" << std::endl;
}

void GuiBuilder::Initialize()
{
    auto renderer = RendererCore::GetInstance(mainWindow->centralWidget());
    if (renderer && renderer->GetUIManager()) {
        renderer->GetUIManager()->SetGUIBuilder(this);
    }
}

void GuiBuilder::Cleanup()
{
    if (scene && scene->GetUIManager()) {
        scene->GetUIManager()->SetGUIBuilder({});
    }
    scene = nullptr;
}

void GuiBuilder::SetEntity(BaseEntity* e)
{
    this->_entity = e;
}

BaseEntity* GuiBuilder::GetEntity()
{
    return this->_entity;
}

void GuiBuilder::AddElement(const std::string& title, const std::string& propTitle)
{
    if (propTitle.find("Transform3D") != std::string::npos) {
        auto transform = new Transform3DProperty(mainWindow->centralWidget());
        transform->SetEntity(GetEntity());
        rightPanel->addElement("PROPERTIES", transform);
    }
	else if (propTitle.find("Collider") != std::string::npos) {
		auto collider = new ColliderProperty(mainWindow->centralWidget());
		collider->SetEntity(GetEntity());
		rightPanel->addElement("PROPERTIES", collider);
	}
    else if (propTitle.find("LOD") != std::string::npos)
    {
		auto lod = new LODProperty(mainWindow->centralWidget());
		lod->SetEntity(GetEntity());
		rightPanel->addElement("PROPERTIES", lod);
    }
}

void GuiBuilder::RemoveAllElements()
{
	rightPanel->removeAllElements("PROPERTIES");
    rightPanel->update();
}
