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

    QWidget* widgetContainer = new QWidget();
    QVBoxLayout* _mainLayout = new QVBoxLayout(widgetContainer);
    _mainLayout->addWidget(new QLabel("Position"));
    _mainLayout->addWidget(new Vec3Property());
    _mainLayout->addWidget(new QLabel("Rotation"));
    _mainLayout->addWidget(new Vec3Property());
    _mainLayout->addWidget(new QLabel("Scale"));
    _mainLayout->addWidget(new Vec3Property());
    _mainLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    _mainLayout->setSpacing(0);

	PropertyGroup* group = new PropertyGroup("Transform");
	group->getLayout()->addWidget(widgetContainer);

	PropertyGroup* group2 = new PropertyGroup("RigidBody");
	group2->getLayout()->addWidget(new QLabel("Constraits"));
	group2->getLayout()->addWidget(new QLabel("Mass"));
	group2->getLayout()->addWidget(new QLabel("Friction"));
	group2->getLayout()->addWidget(new QLabel("Restitution"));
	group2->getLayout()->addWidget(new QLabel("Linear Damping"));
	group2->getLayout()->addWidget(new QLabel("Angular Damping"));
	group2->getLayout()->addWidget(new QLabel("Linear Factor"));
	group2->getLayout()->addWidget(new QLabel("Angular Factor"));
	group2->getLayout()->addWidget(new QLabel("Gravity"));
	group2->getLayout()->addWidget(new QLabel("Kinematic"));
	group2->getLayout()->addWidget(new QLabel("Sleeping"));
	group2->getLayout()->addWidget(new QLabel("Collision Group"));
	group2->getLayout()->addWidget(new QLabel("Collision Mask"));
	group2->getLayout()->addWidget(new QLabel("Collision Response"));
	group2->getLayout()->addWidget(new QLabel("Collision Shape"));
	group2->getLayout()->addWidget(new QLabel("Collision Shape Type"));
	group2->getLayout()->addWidget(new QLabel("Collision Shape Size"));
	group2->getLayout()->addWidget(new QLabel("Collision Shape Offset"));
	group2->getLayout()->addWidget(new QLabel("Collision Shape Rotation"));
	group2->getLayout()->addWidget(new QLabel("Collision Shape Mass"));
	group2->getLayout()->addWidget(new QLabel("Collision Shape Friction"));
	group2->getLayout()->addWidget(new QLabel("Collision Shape Restitution"));
	group2->getLayout()->addWidget(new QLabel("Collision Shape Linear Damping"));
	group2->getLayout()->addWidget(new QLabel("Collision Shape Angular Damping"));
	group2->getLayout()->addWidget(new QLabel("Collision Shape Linear Factor"));
	group2->getLayout()->addWidget(new QLabel("Collision Shape Angular Factor"));
	group2->getLayout()->addWidget(new QLabel("Collision Shape Gravity"));
	group2->getLayout()->addWidget(new QLabel("Collision Shape Kinematic"));
	group2->getLayout()->addWidget(new QLabel("Collision Shape Sleeping"));

	PropertyGroup* group3 = new PropertyGroup("RigidBody");
    group3->getLayout()->addWidget(new QLabel("Constraits"));
    group3->getLayout()->addWidget(new QLabel("Mass"));
    group3->getLayout()->addWidget(new QLabel("Friction"));
    group3->getLayout()->addWidget(new QLabel("Restitution"));
    group3->getLayout()->addWidget(new QLabel("Linear Damping"));
    group3->getLayout()->addWidget(new QLabel("Angular Damping"));
    group3->getLayout()->addWidget(new QLabel("Linear Factor"));
    group3->getLayout()->addWidget(new QLabel("Angular Factor"));
    group3->getLayout()->addWidget(new QLabel("Gravity"));
    group3->getLayout()->addWidget(new QLabel("Kinematic"));
    group3->getLayout()->addWidget(new QLabel("Sleeping"));
    group3->getLayout()->addWidget(new QLabel("Collision Group"));
    group3->getLayout()->addWidget(new QLabel("Collision Mask"));
    group3->getLayout()->addWidget(new QLabel("Collision Response"));
    group3->getLayout()->addWidget(new QLabel("Collision Shape"));
    group3->getLayout()->addWidget(new QLabel("Collision Shape Type"));
    group3->getLayout()->addWidget(new QLabel("Collision Shape Size"));
    group3->getLayout()->addWidget(new QLabel("Collision Shape Offset"));
    group3->getLayout()->addWidget(new QLabel("Collision Shape Rotation"));
    group3->getLayout()->addWidget(new QLabel("Collision Shape Mass"));
    group3->getLayout()->addWidget(new QLabel("Collision Shape Friction"));
    group3->getLayout()->addWidget(new QLabel("Collision Shape Restitution"));
    group3->getLayout()->addWidget(new QLabel("Collision Shape Linear Damping"));
    group3->getLayout()->addWidget(new QLabel("Collision Shape Angular Damping"));
    group3->getLayout()->addWidget(new QLabel("Collision Shape Linear Factor"));
    group3->getLayout()->addWidget(new QLabel("Collision Shape Angular Factor"));
    group3->getLayout()->addWidget(new QLabel("Collision Shape Gravity"));
    group3->getLayout()->addWidget(new QLabel("Collision Shape Kinematic"));
    group3->getLayout()->addWidget(new QLabel("Collision Shape Sleeping"));

	PropertyGroup* group4 = new PropertyGroup("RigidBody");
    group4->getLayout()->addWidget(new QLabel("Constraits"));
    group4->getLayout()->addWidget(new QLabel("Mass"));
    group4->getLayout()->addWidget(new QLabel("Friction"));
    group4->getLayout()->addWidget(new QLabel("Restitution"));
    group4->getLayout()->addWidget(new QLabel("Linear Damping"));
    group4->getLayout()->addWidget(new QLabel("Angular Damping"));
    group4->getLayout()->addWidget(new QLabel("Linear Factor"));
    group4->getLayout()->addWidget(new QLabel("Angular Factor"));
    group4->getLayout()->addWidget(new QLabel("Gravity"));
    group4->getLayout()->addWidget(new QLabel("Kinematic"));
    group4->getLayout()->addWidget(new QLabel("Sleeping"));
    group4->getLayout()->addWidget(new QLabel("Collision Group"));
    group4->getLayout()->addWidget(new QLabel("Collision Mask"));
    group4->getLayout()->addWidget(new QLabel("Collision Response"));
    group4->getLayout()->addWidget(new QLabel("Collision Shape"));
    group4->getLayout()->addWidget(new QLabel("Collision Shape Type"));
    group4->getLayout()->addWidget(new QLabel("Collision Shape Size"));
    group4->getLayout()->addWidget(new QLabel("Collision Shape Offset"));
    group4->getLayout()->addWidget(new QLabel("Collision Shape Rotation"));
    group4->getLayout()->addWidget(new QLabel("Collision Shape Mass"));
    group4->getLayout()->addWidget(new QLabel("Collision Shape Friction"));
    group4->getLayout()->addWidget(new QLabel("Collision Shape Restitution"));
    group4->getLayout()->addWidget(new QLabel("Collision Shape Linear Damping"));
    group4->getLayout()->addWidget(new QLabel("Collision Shape Angular Damping"));
    group4->getLayout()->addWidget(new QLabel("Collision Shape Linear Factor"));
    group4->getLayout()->addWidget(new QLabel("Collision Shape Angular Factor"));
    group4->getLayout()->addWidget(new QLabel("Collision Shape Gravity"));
    group4->getLayout()->addWidget(new QLabel("Collision Shape Kinematic"));
    group4->getLayout()->addWidget(new QLabel("Collision Shape Sleeping"));

	PropertyGroup* group5 = new PropertyGroup("RigidBody");
    group5->getLayout()->addWidget(new QLabel("Constraits"));
    group5->getLayout()->addWidget(new QLabel("Mass"));
    group5->getLayout()->addWidget(new QLabel("Friction"));
    group5->getLayout()->addWidget(new QLabel("Restitution"));
    group5->getLayout()->addWidget(new QLabel("Linear Damping"));
    group5->getLayout()->addWidget(new QLabel("Angular Damping"));
    group5->getLayout()->addWidget(new QLabel("Linear Factor"));
    group5->getLayout()->addWidget(new QLabel("Angular Factor"));
    group5->getLayout()->addWidget(new QLabel("Gravity"));
    group5->getLayout()->addWidget(new QLabel("Kinematic"));
    group5->getLayout()->addWidget(new QLabel("Sleeping"));
    group5->getLayout()->addWidget(new QLabel("Collision Group"));
    group5->getLayout()->addWidget(new QLabel("Collision Mask"));
    group5->getLayout()->addWidget(new QLabel("Collision Response"));
    group5->getLayout()->addWidget(new QLabel("Collision Shape"));
    group5->getLayout()->addWidget(new QLabel("Collision Shape Type"));
    group5->getLayout()->addWidget(new QLabel("Collision Shape Size"));
    group5->getLayout()->addWidget(new QLabel("Collision Shape Offset"));
    group5->getLayout()->addWidget(new QLabel("Collision Shape Rotation"));
    group5->getLayout()->addWidget(new QLabel("Collision Shape Mass"));
    group5->getLayout()->addWidget(new QLabel("Collision Shape Friction"));
    group5->getLayout()->addWidget(new QLabel("Collision Shape Restitution"));
    group5->getLayout()->addWidget(new QLabel("Collision Shape Linear Damping"));
    group5->getLayout()->addWidget(new QLabel("Collision Shape Angular Damping"));
    group5->getLayout()->addWidget(new QLabel("Collision Shape Linear Factor"));
    group5->getLayout()->addWidget(new QLabel("Collision Shape Angular Factor"));
    group5->getLayout()->addWidget(new QLabel("Collision Shape Gravity"));
    group5->getLayout()->addWidget(new QLabel("Collision Shape Kinematic"));
    group5->getLayout()->addWidget(new QLabel("Collision Shape Sleeping"));

    rightPanel->addPage("PROPERTIES", group);
    rightPanel->addPage("TEST", testFrame.release());
    rightPanel->addPage("TEST2", testFrame2.release());
    rightPanel->addPage("HIERARCHY", new QLabel("Prova!"));
    
	rightPanel->addElement("PROPERTIES", group2);
	// rightPanel->addElement("PROPERTIES", group3);
	// rightPanel->addElement("PROPERTIES", group4);
	// rightPanel->addElement("PROPERTIES", group5);

    topPanel->setStyleSheet("background-color: rgb(25, 25, 25);" "border-radius: 10px");
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