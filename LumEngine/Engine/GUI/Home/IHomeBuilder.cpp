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
		emit projectAdded();
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
	_projectButtonsContainer = new QFrame(_mainContainer);
	_connectionButtonsContainer = new QFrame(_mainContainer);
    _buttonsContainer = new QFrame(_mainSplitter);

    _mainLayout = new QVBoxLayout(_mainContainer);
	_mainButtonsLayout = new QHBoxLayout(_buttonsContainer);
    _projectButtonsLayout = new QHBoxLayout(_projectButtonsContainer);
    _connectionButtonsLayout = new QHBoxLayout(_connectionButtonsContainer);
    
	_projectsScrollArea = new QScrollArea(_mainContainer);

    _newProjectButton = new BaseButton(_buttonsContainer);
	_openProjectButton = new BaseButton(_buttonsContainer);
	_liveEditorButton = new BaseButton(_buttonsContainer);

    QWidget* scrollContent = new QWidget;
    QVBoxLayout* scrollLayout = new QVBoxLayout(scrollContent);
    scrollLayout->setAlignment(Qt::AlignTop);  // Allinea i progetti in alto
    scrollLayout->setSpacing(20);  // Spazio tra i progetti
    scrollLayout->setContentsMargins(5, 5, 5, 5);

    _projectsScrollArea->setWidget(scrollContent);
    _projectsScrollArea->setWidgetResizable(true);
    _projectsScrollArea->setStyleSheet("QScrollArea { border: none; background: transparent; }");

    // Aggiungi lo ScrollArea al mainLayout
    _mainLayout->addWidget(_projectsScrollArea);

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

	_projectButtonsContainer->setContentsMargins(0, 0, 0, 0);
	_connectionButtonsContainer->setContentsMargins(0, 0, 0, 0);

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

	_liveEditorButton->setStyleSheet(
		"QPushButton {"
		"background-color: rgba(45, 45, 45, 1);"
		"font-size: 16px;"
		"border-radius: 10px;"
		"}"
	);
	_liveEditorButton->setText("LIVE");
	_liveEditorButton->setCursor(Qt::PointingHandCursor);
	_liveEditorButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	_liveEditorButton->setFixedSize(100, 50);

	_projectButtonsLayout->setAlignment(Qt::AlignLeft);
	_projectButtonsLayout->addWidget(_newProjectButton, Qt::AlignLeft);
	_projectButtonsLayout->addWidget(_openProjectButton, Qt::AlignLeft);

	_connectionButtonsLayout->setAlignment(Qt::AlignRight);
	_connectionButtonsLayout->addWidget(_liveEditorButton, Qt::AlignRight);
	_projectButtonsLayout->setContentsMargins(0, 0, 0, 0);
    _projectButtonsLayout->setSpacing(5);
	_connectionButtonsLayout->setContentsMargins(0, 0, 0, 0);
    _connectionButtonsLayout->setSpacing(5);

	_mainButtonsLayout->addWidget(_projectButtonsContainer);
	_mainButtonsLayout->addWidget(_connectionButtonsContainer);
	_mainButtonsLayout->setContentsMargins(0, 0, 0, 0);
	_mainButtonsLayout->setSpacing(0);

    GetProjects();

	_window->show();

	QObject::connect(_newProjectButton, &BaseButton::clicked, _projectManager, &ProjectManager::onNewProjectClicked);
	QObject::connect(_openProjectButton, &BaseButton::clicked, _projectManager, &ProjectManager::onOpenProjectClicked);
	QObject::connect(_projectManager, &ProjectManager::projectAdded, _projectManager, [&]()
		{
			AddProject(_window->GetProjectPath());
		});
}

void HomeBuilder::AddProject(const std::string& projectPath)
{
    QSettings settings("LumE", "Projects");

    // Ottieni la lista esistente dei progetti
    QStringList projects = settings.value("ProjectsList").toStringList();

    // Aggiungi il nuovo progetto se non esiste già
    QString newProject = QString::fromStdString(projectPath);
    if (!projects.contains(newProject)) {
        projects.append(newProject);
        settings.setValue("ProjectsList", projects);
    }
}

void HomeBuilder::GetProjects() {
    QSettings settings("LumE", "Projects");
    QStringList projects = settings.value("ProjectsList").toStringList();

    for (const QString& projectPath : projects) {
        CreateProjectWidget(projectPath.toStdString());
    }
}

void HomeBuilder::CreateProjectWidget(const std::string& projectPath)
{
    ProjectButton* projectFrame = new ProjectButton();
	projectFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    projectFrame->setFixedHeight(100);
    projectFrame->setStyleSheet(R"(
        QFrame { 
            background-color: rgb(20, 20, 20); 
            border-radius: 10px;
        }
        QFrame:hover {
            background-color: rgb(40, 40, 40);
        }
    )");
    projectFrame->SetProjectPath(QString::fromStdString(projectPath));
    _projects.append(projectFrame);

    // Layout per il progetto
    QVBoxLayout* layout = new QVBoxLayout(projectFrame);
    _projectsLayout.append(layout);

    // Nome del progetto (usa il nome della directory)
    QFileInfo fileInfo(QString::fromStdString(projectPath));
    QLabel* nameLabel = new QLabel(fileInfo.fileName());
    nameLabel->setStyleSheet("QLabel { color: white; }");
    _projectsName.append(nameLabel);

    layout->addWidget(nameLabel);

    // Aggiungi il frame all'area di scroll
    if (_projectsScrollArea) {
        QWidget* scrollContent = _projectsScrollArea->widget();
        if (QVBoxLayout* scrollLayout = qobject_cast<QVBoxLayout*>(scrollContent->layout())) {
            scrollLayout->addWidget(projectFrame);
        }
    }

    projectFrame->SetProjectName(fileInfo.fileName());
    projectFrame->SetProjectDescription("Description");

    QObject::connect(projectFrame, &ProjectButton::clicked, _projectManager, [this, projectFrame]() {
        QString path = projectFrame->GetProjectPath();
        if (!path.isEmpty()) {
            std::cout << "Loading with path: " << path.toStdString() << std::endl;
			_window->LoadProjectFromPath(path.toStdString());
			_window->close();
			_projectManager->RunEditor();
        }
        });
}
