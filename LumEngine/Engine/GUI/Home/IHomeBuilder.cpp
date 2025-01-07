#include "IHomeBuilder.h"

ProjectManager::ProjectManager(QWidget* parent) : QWidget(parent)
{
}

void ProjectManager::SetMainWindow(std::shared_ptr<MainWindow> mainWindow)
{
	_mainWindow = mainWindow;
}

void ProjectManager::onNewProjectClicked()
{
	if (_mainWindow->createNewProject())
	{
		std::cout << "New project created!" << std::endl;
        _mainWindow->close();
        RunEditor();
	}
}

void ProjectManager::onOpenProjectClicked()
{
    if (_mainWindow->loadProject())
	{
		std::cout << "Project loaded!" << std::endl;
        _mainWindow->close();
		RunEditor();
	}
}

void ProjectManager::RunEditor()
{
    SetEnvironmentVariable(L"COREHOST_TRACE", L"1");
    SetEnvironmentVariable(L"COREHOST_TRACEFILE", L"corehost.log");
    SetEnvironmentVariable(L"COREHOST_TRACE_VERBOSITY", L"4");

    _loadingWindow = new LoadingWindow();
    _loadingWindow->show();

    _loadingThread = std::make_shared<LoadingThread>();

    _loadingWindow->SetThread(_loadingThread.get());

    // Collega il segnale di aggiornamento del progresso alla finestra di caricamento
    QObject::connect(_loadingThread.get(), &LoadingThread::progressUpdated, _loadingWindow, &LoadingWindow::setProgress);

    QSurfaceFormat format;
    format.setVersion(4, 5);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(24);
    format.setOption(QSurfaceFormat::DebugContext);
    format.setSwapInterval(1); // VSync (1 = attivato, 0 = disattivato
    QSurfaceFormat::setDefaultFormat(format);

    _loadingWindow->setProgress("Init GUI...", 100);

    // Usa QMetaObject::invokeMethod per creare GuiBuilder nel thread principale
    QObject::connect(_loadingThread.get(), &LoadingThread::finished, [this]() {
        QMetaObject::invokeMethod(qApp, [this]() {
            _loadingWindow->close();
            _builder = std::make_shared<GuiBuilder>();

            _builder->getPlayButton()->SetScriptRunner(_loadingThread->getStart(), _loadingThread->getUpdate());
            _builder->getMainWindow()->SetThread(_loadingThread.get());

            _builder->Initialize();

            const std::string& path = _mainWindow->GetProjectPath();
            _builder->SetProjectPath(path);

            try {
                QString projPath = QString::fromStdString(_mainWindow->GetProjectPath());
                QString projName = QDir(projPath).dirName();
                QString title = QString("LumE - %1").arg(projName.isEmpty() ? "Untitled" : projName);
                _builder->getMainWindow()->setWindowTitle(title);
            }
            catch (const std::exception& e) {
                qDebug() << "Error setting window title:" << e.what();
                _builder->getMainWindow()->setWindowTitle("LumE");
            }

            QApplication::setStyle(QStyleFactory::create("Fusion")); // Tema Fusion per Windows
            QIcon icon("Resources/Assets/LumEngine_Logo.ico");
            if (!icon.isNull()) {
                _builder->getMainWindow()->setWindowIcon(icon);
            }

            std::cout << "Path from RunEditor: " << _mainWindow->GetProjectPath() << std::endl;

            _builder->getMainWindow()->setWindowState(Qt::WindowMaximized);

        }, Qt::QueuedConnection);
    });

    _loadingThread->start();

    QObject::connect(qApp, &QApplication::aboutToQuit, [&]() {
        if (_builder) {
            auto renderer = RendererCore::GetInstance();
            if (renderer && renderer->GetUIManager()) {
                renderer->GetUIManager()->SetGUIBuilder(nullptr);
            }
            _builder.reset();
        }
    });
}

HomeBuilder::HomeBuilder()
{
	_projectManager = new ProjectManager(nullptr);
	_window = std::make_shared<MainWindow>(nullptr);
	_loadingThread = new LoadingThread(nullptr);
    _mainSplitter = new QSplitter(_window->centralWidget());
	_mainContainer = new QFrame(_mainSplitter);
    _buttonsContainer = new QFrame(_mainSplitter);
	_mainButtonsLayout = new QHBoxLayout(_buttonsContainer);
    _projectButtonsLayout = new QHBoxLayout();
    _connectionButtonsLayout = new QHBoxLayout();
    _newProjectButton = new BaseButton(_buttonsContainer);
	_openProjectButton = new BaseButton(_buttonsContainer);
	_connectToButton = new BaseButton(_buttonsContainer);
	_liveEditorButton = new BaseButton(_buttonsContainer);

    _projectManager->SetMainWindow(_window);

	_window->SetThread(_loadingThread);
	_window->setGeometry(200, 100, 800, 600);
	_window->setWindowTitle("LumE - Projects");
    _window->setStyleSheet("QMainWindow {"
		"background-color: rgb(20, 20, 20);"
		"color: rgb(255, 255, 255);"
        "}");
	_window->GetMainLayout()->addWidget(_mainSplitter, Qt::AlignLeft);
	_window->GetMainLayout()->setSpacing(0);
    _window->setContentsMargins(0, 0, 0, 0);
	_window->centralWidget()->setContentsMargins(0, 0, 0, 0);

	_mainSplitter->setOrientation(Qt::Vertical);

	_buttonsContainer->setFixedHeight(60);
	_buttonsContainer->setStyleSheet("QFrame {"
		"background-color: rgb(20, 20, 20);"
		"color: rgb(255, 255, 255);"
		"}"
	);
	_buttonsContainer->setContentsMargins(0, 0, 0, 0);

	_newProjectButton->setStyleSheet(
        "QPushButton {"
        "background-color: rgba(0, 127, 255, 1);"
        "font-size: 16px;"
        "border-radius: 10px;"
        "}"
    );
	_newProjectButton->setText("NEW");
	_newProjectButton->setCursor(Qt::PointingHandCursor);
	_newProjectButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	_newProjectButton->setFixedSize(100, 50);

	_openProjectButton->setStyleSheet(
		"QPushButton {"
		"background-color: rgba(0, 127, 255, 1);"
        "font-size: 16px;"
        "border-radius: 10px;"
		"}"
    );
	_openProjectButton->setText("OPEN");
	_openProjectButton->setCursor(Qt::PointingHandCursor);
    _openProjectButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	_openProjectButton->setFixedSize(100, 50);

	_connectToButton->setStyleSheet(
		"QPushButton {"
		"background-color: rgba(35, 35, 35, 1);"
		"font-size: 16px;"
		"border-radius: 10px;"
		"}"
	);
	_connectToButton->setText("CONNECT TO");
	_connectToButton->setCursor(Qt::PointingHandCursor);
	_connectToButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	_connectToButton->setFixedSize(100, 50);

	_liveEditorButton->setStyleSheet(
		"QPushButton {"
		"background-color: rgba(35, 35, 35, 1);"
		"font-size: 16px;"
		"border-radius: 10px;"
		"}"
	);
	_liveEditorButton->setText("LIVE EDITOR");
	_liveEditorButton->setCursor(Qt::PointingHandCursor);
	_liveEditorButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	_liveEditorButton->setFixedSize(100, 50);

	_projectButtonsLayout->addWidget(_newProjectButton, Qt::AlignLeft);
	_projectButtonsLayout->addWidget(_openProjectButton, Qt::AlignLeft);
	_connectionButtonsLayout->addWidget(_connectToButton, Qt::AlignRight);
	_connectionButtonsLayout->addWidget(_liveEditorButton, Qt::AlignRight);
	_projectButtonsLayout->setContentsMargins(0, 0, 0, 0);
    _projectButtonsLayout->setSpacing(5);
	_connectionButtonsLayout->setContentsMargins(0, 0, 0, 0);
    _connectionButtonsLayout->setSpacing(5);

	_mainButtonsLayout->addLayout(_projectButtonsLayout);
	_mainButtonsLayout->addLayout(_connectionButtonsLayout);
	_mainButtonsLayout->setContentsMargins(0, 0, 0, 0);
	_mainButtonsLayout->setSpacing(0);

	_window->show();

	QObject::connect(_newProjectButton, &BaseButton::clicked, _projectManager, &ProjectManager::onNewProjectClicked);
	QObject::connect(_openProjectButton, &BaseButton::clicked, _projectManager, &ProjectManager::onOpenProjectClicked);
}