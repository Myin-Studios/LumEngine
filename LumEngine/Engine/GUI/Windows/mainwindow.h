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

    QHBoxLayout* getMainLayout() const;

    void setThread(LoadingThread* thread) { this->loadingThread = thread; }

private:
    void createNewProject();
    void loadProject();
    void buildProject();

    ScriptPathLoader* splc = nullptr;
    LoadingThread* loadingThread = nullptr;
    QString assemblyPath = "";
    QWidget* m_CentralWidget = nullptr;
    QHBoxLayout* m_MainLayout = nullptr;

signals:
    void assemblyPathProvided(QString path);
};
