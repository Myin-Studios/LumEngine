#pragma once
#include <QHBoxLayout>
#include <QMainWindow>
#include <QShortcut>
#include <QKeySequence>
#include <QFileDialog>
#include <QMessageBox>
#include <QString>
#include <iostream>

#include "Engine/Core/hostInitializer.h"
#include "Engine/Core/AssemblyLoader/ScriptPathLoader.h"

using namespace ScriptLoaderCore;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    QHBoxLayout* GetMainLayout() const;

    void SetThread(LoadingThread* thread) { this->loadingThread = thread; }

    bool createNewProject();
    bool loadProject();

	void LoadProjectFromPath(const std::string& path);

    const std::string& GetProjectPath() const;

private:
    
    void buildProject();

    ScriptPathLoader* splc = nullptr;
    LoadingThread* loadingThread = nullptr;
    QString assemblyPath = "";
    QWidget* m_CentralWidget = nullptr;
    QHBoxLayout* m_MainLayout = nullptr;

signals:
    void assemblyPathProvided(QString path);
};
