#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_CentralWidget = new QWidget(this);
    setCentralWidget(m_CentralWidget);

    m_MainLayout = new QHBoxLayout();
    m_CentralWidget->setLayout(m_MainLayout);

    m_CentralWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    splc = new ScriptPathLoader();

    QShortcut* shortcut = new QShortcut(QKeySequence("Ctrl+N"), this);
    connect(shortcut, &QShortcut::activated, this, &MainWindow::createNewProject);
    
    QShortcut* loadShortcut = new QShortcut(QKeySequence("Ctrl+O"), this);
    connect(loadShortcut, &QShortcut::activated, this, &MainWindow::loadProject);
    
    QShortcut* buildShortcut = new QShortcut(QKeySequence("Ctrl+B"), this);
    connect(buildShortcut, &QShortcut::activated, this, &MainWindow::buildProject);
}

MainWindow::~MainWindow()
{
    delete m_CentralWidget;
    delete m_MainLayout;
}

QHBoxLayout* MainWindow::GetMainLayout() const
{
    return m_MainLayout;
}

bool MainWindow::createNewProject()
{
    connect(this, &MainWindow::assemblyPathProvided, loadingThread, &LoadingThread::LoadAssembly);

    if (loadingThread != nullptr)
    {
        QString dir = QFileDialog::getExistingDirectory(this, tr("Create New Project"), "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

        if (!dir.isEmpty()) {
            QDir projectDir(dir);
            if (!projectDir.exists()) {
                // Creare la directory se non esiste
                if (projectDir.mkpath(".")) {
                    std::cout << "Project created successfully" << std::endl;
                }
                else {
                    std::cout << "Failed to create project." << std::endl;
                    return false;
                }
            }

            // Impostare il percorso del progetto
            assemblyPath = dir;
            
            splc->setPath(dir);
            splc->createProject(dir.toStdString(), projectDir.dirName().toStdString());
            
            emit assemblyPathProvided(dir);

			loadingThread->start();

            return true;
        }
        else {
            QMessageBox::warning(this, "Warning", "No directory selected!");

            return false;
        }

    }
    else std::cerr << "LoadingThread is null!" << std::endl;

    return false;
}

bool MainWindow::loadProject()
{
    connect(this, &MainWindow::assemblyPathProvided, loadingThread, &LoadingThread::LoadAssembly);

    if (loadingThread != nullptr)
    {
        QString dir = QFileDialog::getExistingDirectory(this, tr("Load Project"), "/home");

        if (!dir.isEmpty()) {
            if (assemblyPath != dir)
            {
                assemblyPath = dir;

                splc->clean(assemblyPath.toStdString());
                splc->restore(dir.toStdString());
                splc->build(dir.toStdString());

                emit assemblyPathProvided(dir);

                return true;
            }
            else
            {
                std::cout << "Project already loaded!" << std::endl;
                return false;
            }
        }
        else {
            QMessageBox::warning(this, "Warning", "No directory selected!");
            return false;
        }
    }
    else std::cerr << "LoadingThread is null!" << std::endl;
    
    return false;
}

void MainWindow::LoadProjectFromPath(const std::string& path)
{
	assemblyPath = QString::fromStdString(path);
	splc->build(path);
	emit assemblyPathProvided(assemblyPath);

	std::cout << "Assembly path: " << assemblyPath.toStdString() << std::endl;
}

const std::string& MainWindow::GetProjectPath() const
{
    static std::string path;
    path = assemblyPath.toStdString();
    return path;
}

void MainWindow::buildProject()
{
    if (assemblyPath.isEmpty())
    {
        QMessageBox::warning(this, "Warning", "No project loaded before!");
        return;
    }

    connect(this, &MainWindow::assemblyPathProvided, loadingThread, &LoadingThread::LoadAssembly);

    emit assemblyPathProvided("");

    splc->restore(assemblyPath.toStdString());
    splc->build(assemblyPath.toStdString());

	emit assemblyPathProvided(assemblyPath);
}