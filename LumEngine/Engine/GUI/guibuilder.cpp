#include "guibuilder.h"

GuiBuilder::GuiBuilder()
{
    mainWindow = std::make_unique<MainWindow>();
    topPanel = std::make_unique<Panel>(mainWindow->centralWidget());
    leftPanel = std::make_unique<VerticalPanel>(mainWindow->centralWidget());
    rightPanel = std::make_unique<BasePanel>("test");
    tBtn = std::make_unique<TabsButton>(nullptr);
    playButton = std::make_unique<PlayButton>(nullptr);
    centralLayout = std::make_unique<QVBoxLayout>(nullptr);
    scene = RendererCore::GetInstance(mainWindow->centralWidget());
    scene->SetRunningThread(playButton->GetScriptRunner());
    console = std::make_unique<Console>(mainWindow->centralWidget());
    // outputRedirector = std::make_unique<RedirectStreamBuf>(console.get());
    sceneConsoleSplitter = std::make_unique<QSplitter>();
    rightPanelSplitter = std::make_unique<QSplitter>();
    mainContainer = std::make_unique<QWidget>();

    mainWindow->setStyleSheet("background-color: rgb(25, 25, 25);");

    auto configureButton = [](QPushButton* button) {
        button->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
        button->setMinimumSize(0, button->sizeHint().height());
        };

    lightingSettingsButton = std::make_unique<BaseButton>();
    lightingSettingsButton->setText("");
    lightingSettingsButton->setIcon(QIcon("Resources/Icons/New/LumE_LightSettingsHover.svg"));
    lightingSettingsButton->setIconSize(QSize(20, 20));
    lightingSettingsButton->setCursor(Qt::PointingHandCursor);

    hierarchyButton = std::make_unique<BaseButton>();
    hierarchyButton->setText("");
    hierarchyButton->setIcon(QIcon("Resources/Icons/New/LumE_HierarchyHover.svg"));
    hierarchyButton->setIconSize(QSize(20, 20));
    hierarchyButton->setCursor(Qt::PointingHandCursor);

    configureButton(lightingSettingsButton.get());
    configureButton(hierarchyButton.get());

    playButton->setIconSize(QSize(20, 20));
    tBtn->setIconSize(QSize(20, 20));

    scene->setStyleSheet("border-radius: 30px");
    scene->setGeometry(scene->x(), scene->y(), 800, 600);
    scene->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    console->setStyleSheet("background-color: rgb(20, 20, 20);");

    // Setting window properties
    mainWindow->setGeometry(mainWindow->x(), mainWindow->y(), 1024, 768);
    mainWindow->setWindowIcon(QIcon(":/Icons/CMEngine_Logo.svg"));

    leftPanel->setStyleSheet("background-color: rgb(20, 20, 20);" "border-radius: 10px");
    leftPanel->setFixedWidth(40);
    leftPanel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    leftPanel->getLayout()->addWidget(tBtn.get());
    leftPanel->getLayout()->addWidget(lightingSettingsButton.get(), 0, Qt::AlignCenter);
    leftPanel->getLayout()->addWidget(hierarchyButton.get());

    topPanel->setStyleSheet("background-color: rgb(20, 20, 20);" "border-radius: 10px");
    topPanel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    topPanel->getLayout()->addWidget(playButton.get(), 0, Qt::AlignLeft);

    sceneConsoleSplitter->addWidget(scene);
    sceneConsoleSplitter->addWidget(console.get());
    sceneConsoleSplitter->setOrientation(Qt::Vertical);

    centralLayout->addWidget(topPanel.get(), 0, Qt::AlignTop);
    centralLayout->addWidget(sceneConsoleSplitter.get());

    centralLayout->setContentsMargins(QMargins(0, 0, 0, 0));

    mainContainer->setLayout(centralLayout.get());

    rightPanelSplitter->addWidget(leftPanel.get());
    rightPanelSplitter->addWidget(mainContainer.get());
    rightPanelSplitter->addWidget(rightPanel.get());

    mainWindow->getMainLayout()->addWidget(rightPanelSplitter.get());

    mainWindow->show();
    leftPanel->show();

    // std::cout.rdbuf(outputRedirector.get());
    // std::cerr.rdbuf(outputRedirector.get());
}

GuiBuilder::~GuiBuilder()
{
    if (scene != nullptr)
    {
        delete scene;
        scene = nullptr;
    }
}