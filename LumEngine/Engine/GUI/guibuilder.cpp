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

    // std::unique_ptr<QFrame> testFrame = std::make_unique<QFrame>();
    // testFrame->setStyleSheet("QFrame { background-color: rgb(255, 0, 255); }");
    // 
    // std::unique_ptr<QFrame> testFrame2 = std::make_unique<QFrame>();
    // testFrame2->setStyleSheet("QFrame { background-color: rgb(0, 255, 255); }");
    // 
    // PropertyGroup* group = new PropertyGroup("Transform", mainWindow);
    // group->addElement(new QLabel("Position"), Qt::AlignTop);
    // group->addElement(new Vec3Property(), Qt::AlignTop);
    // group->addElement(new QLabel("Rotation"), Qt::AlignTop);
    // group->addElement(new Vec3Property(), Qt::AlignTop);
    // group->addElement(new QLabel("Scale"), Qt::AlignTop);
    // group->addElement(new Vec3Property(), Qt::AlignTop);
    // 
    // PropertyGroup* transformGroup = new PropertyGroup("Transform");
    // transformGroup->addElement(new QLabel("Position"), Qt::AlignTop);
    // transformGroup->addElement(new Vec3Property(), Qt::AlignTop);
    // transformGroup->addElement(new QLabel("Rotation"), Qt::AlignTop);
    // transformGroup->addElement(new Vec3Property(), Qt::AlignTop);
    // transformGroup->addElement(new QLabel("Scale"), Qt::AlignTop);
    // transformGroup->addElement(new Vec3Property(), Qt::AlignTop);
    // 
    // PropertyGroup* group2 = new PropertyGroup("RigidBody");
    // QStringList labels = { "Constraits", "Mass", "Friction", "Restitution", "Linear Damping",
    //                       "Angular Damping", "Linear Factor", "Angular Factor", "Gravity",
    //                       "Kinematic", "Sleeping", "Collision Group", "Collision Mask",
    //                       "Collision Response", "Collision Shape", "Collision Shape Type",
    //                       "Collision Shape Size", "Collision Shape Offset", "Collision Shape Rotation",
    //                       "Collision Shape Mass", "Collision Shape Friction", "Collision Shape Restitution",
    //                       "Collision Shape Linear Damping", "Collision Shape Angular Damping",
    //                       "Collision Shape Linear Factor", "Collision Shape Angular Factor",
    //                       "Collision Shape Gravity", "Collision Shape Kinematic", "Collision Shape Sleeping" };
    // 
    // for (const QString& label : labels) {
    //     group2->addElement(new QLabel(label));
    // }
    // 
    // rightPanel->addPage("PROPERTIES", group);
    // rightPanel->addPage("TEST", testFrame.release());
    // // rightPanel->addPage("TEST2", testFrame2.release());
    // // rightPanel->addPage("HIERARCHY", new QLabel("Prova!"));
    // 
    // rightPanel->addElement("PROPERTIES", group2);
    // rightPanel->removeElement("PROPERTIES", "Transform");

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

void GuiBuilder::AddElement(const std::string& title, const std::string& propTitle)
{
    if (propTitle.find("Transform3D") != std::string::npos)
	{
		rightPanel->addElement("PROPERTIES", new Transform3DProperty(mainWindow->centralWidget()));
	}
}

void GuiBuilder::RemoveAllElements()
{
	rightPanel->removeAllElements("PROPERTIES");
}
