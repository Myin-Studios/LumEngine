#include "guibuilder.h"

GuiBuilder::GuiBuilder()
{
    mainWindow = new MainWindow();
    topPanel = new Panel(mainWindow->centralWidget());
    rightPanel = new Panel(mainWindow->centralWidget());
    tBtn = new TabsButton(nullptr);
    playButton = new PlayButton(nullptr);
    centralLayout = new QVBoxLayout(nullptr);
    scene = new Renderer(mainWindow->centralWidget());

    playButton->setIconSize(QSize(20, 20));
    tBtn->setIconSize(QSize(20, 20));

    scene->setStyleSheet("border-radius: 30px");
    scene->setGeometry(scene->x(), scene->y(), 800, 600);
    scene->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Setting window properties
    mainWindow->setGeometry(mainWindow->x(), mainWindow->y(), 1024, 768);
    mainWindow->setWindowIcon(QIcon(":/Icons/CMEngine_Logo.svg"));

    rightPanel->setStyleSheet("background-color: rgb(20, 20, 20);" "border-radius: 10px");
    rightPanel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    rightPanel->getLayout()->addWidget(tBtn, 0, Qt::AlignTop | Qt::AlignRight);

    topPanel->setStyleSheet("background-color: rgb(20, 20, 20);" "border-radius: 10px");
    topPanel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    topPanel->getLayout()->addWidget(playButton, 0, Qt::AlignLeft);

    centralLayout->addWidget(topPanel, 0, Qt::AlignTop);
    centralLayout->addWidget(scene);

    mainWindow->getMainLayout()->addWidget(rightPanel, 0, Qt::AlignLeft);
    mainWindow->getMainLayout()->addLayout(centralLayout);

    mainWindow->show();
    rightPanel->show();
}

GuiBuilder::~GuiBuilder()
{
    delete scene;

    delete topPanel;
    delete mainWindow;
}