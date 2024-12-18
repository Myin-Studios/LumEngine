#include "guibuilder.h"

GuiBuilder::GuiBuilder()
{
    mainWindow = std::make_unique<MainWindow>();
    topPanel = std::make_unique<VPanel>(mainWindow->centralWidget());
    leftPanel = std::make_unique<VerticalPanel>(mainWindow->centralWidget());
    rightPanel = std::make_unique<BasePanel>();
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
    position = std::make_unique<Vec3Property>();

    mainWindow->setStyleSheet("background-color: rgb(20, 20, 20);");

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

    console->setStyleSheet("background-color: rgb(25, 25, 25);");

    // Setting window properties
    mainWindow->setGeometry(mainWindow->x(), mainWindow->y(), 1024, 768);
    mainWindow->setWindowIcon(QIcon(":/Icons/CMEngine_Logo.svg"));

    leftPanel->setStyleSheet("background-color: rgb(25, 25, 25);" "border-radius: 10px");
    leftPanel->setFixedWidth(40);
    leftPanel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    leftPanel->getLayout()->addWidget(tBtn.get());
    leftPanel->getLayout()->addWidget(lightingSettingsButton.get(), 0, Qt::AlignCenter);
    leftPanel->getLayout()->addWidget(hierarchyButton.get());

    std::unique_ptr<QFrame> testFrame = std::make_unique<QFrame>();
    testFrame->setStyleSheet("QFrame { background-color: rgb(255, 0, 255); }");

    std::unique_ptr<QFrame> testFrame2 = std::make_unique<QFrame>();
    testFrame2->setStyleSheet("QFrame { background-color: rgb(0, 255, 255); }");

	PropertyGroup* group = new PropertyGroup("Transform");
    group->addElement(new QLabel("Position"), Qt::AlignTop);
    group->addElement(new Vec3Property(), Qt::AlignTop);
    group->addElement(new QLabel("Rotation"), Qt::AlignTop);
    group->addElement(new Vec3Property(), Qt::AlignTop);
    group->addElement(new QLabel("Scale"), Qt::AlignTop);
    group->addElement(new Vec3Property(), Qt::AlignTop);

	PropertyGroup* group2 = new PropertyGroup("RigidBody");
    QStringList labels = { "Constraits", "Mass", "Friction", "Restitution", "Linear Damping",
                          "Angular Damping", "Linear Factor", "Angular Factor", "Gravity",
                          "Kinematic", "Sleeping", "Collision Group", "Collision Mask",
                          "Collision Response", "Collision Shape", "Collision Shape Type",
                          "Collision Shape Size", "Collision Shape Offset", "Collision Shape Rotation",
                          "Collision Shape Mass", "Collision Shape Friction", "Collision Shape Restitution",
                          "Collision Shape Linear Damping", "Collision Shape Angular Damping",
                          "Collision Shape Linear Factor", "Collision Shape Angular Factor",
                          "Collision Shape Gravity", "Collision Shape Kinematic", "Collision Shape Sleeping" };
    
    for (const QString& label : labels) {
        group2->addElement(new QLabel(label));
    }

    rightPanel->addPage("PROPERTIES", group);
    // rightPanel->addPage("TEST", testFrame.release());
    // rightPanel->addPage("TEST2", testFrame2.release());
    // rightPanel->addPage("HIERARCHY", new QLabel("Prova!"));
    
    rightPanel->addElement("PROPERTIES", group2);

    qDebug() << group->size() << group2->size();

    topPanel->setStyleSheet("background-color: rgb(25, 25, 25);" "border-radius: 10px");
    topPanel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    topPanel->getLayout()->addWidget(playButton.get(), 0, Qt::AlignLeft);

    sceneConsoleSplitter->addWidget(scene);
    sceneConsoleSplitter->addWidget(console.get());
    sceneConsoleSplitter->setOrientation(Qt::Vertical);

    centralLayout->addWidget(topPanel.get(), 0, Qt::AlignTop);
    centralLayout->addWidget(sceneConsoleSplitter.get());

    centralLayout->setContentsMargins(0, 0, 0, 0);

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