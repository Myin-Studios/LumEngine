#pragma once

#include "Engine/GUI/Loading/loadingWindow.h"
#include "Engine/GUI/guibuilder.h"
#include "Engine/Core/hostInitializer.h"
#include "ProjectButton/ProjectButton.h"

#include <QMainWindow>
#include <QVBoxLayout>
#include "QCoreApplication"
#include "QStyleFactory"
#include <QMetaObject>
#include <QSplitter>
#include <QFrame>
#include <QObject>
#include <QLabel>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QList>
#include <QSettings>

#include <iostream>

class IHomeBuilder
{
public:
	virtual ~IHomeBuilder() = default;

private:
	virtual void RunEditor() = 0;
};

class ProjectManager : public QWidget, public IHomeBuilder
{
	Q_OBJECT

public:
	ProjectManager(QWidget* parent = nullptr);
	void SetMainWindow(std::shared_ptr<MainWindow> mainWindow);

	void RunEditor() override;

signals:
	void projectAdded();

public slots:
	void onNewProjectClicked();
	void onOpenProjectClicked();

private:
	LoadingWindow* _loadingWindow;
	
	std::shared_ptr<MainWindow> _mainWindow;
	std::shared_ptr<LoadingThread> _loadingThread;
	std::shared_ptr<GuiBuilder> _builder;
};

class HomeBuilder
{
public:
	HomeBuilder();

	void AddProject(const std::string& projectPath);
	void GetProjects();

private:
	void CreateProjectWidget(const std::string& projectPath);

	ProjectManager* _projectManager;

	std::shared_ptr<MainWindow> _window;

	LoadingThread* _loadingThread;

	QSplitter* _mainSplitter;

	QLabel* _titleLabel;

	QList<QLabel*> _projectsName;
	QList<ProjectButton*> _projects;
	QList<QVBoxLayout*> _projectsLayout;

	QScrollArea* _projectsScrollArea;

	QFrame* _mainContainer;
	QFrame* _projectsContainer;
	QFrame* _buttonsContainer;
	QFrame* _projectButtonsContainer;
	QFrame* _connectionButtonsContainer;

	QVBoxLayout* _mainLayout;
	QHBoxLayout* _mainButtonsLayout;
	QHBoxLayout* _projectButtonsLayout;
	QHBoxLayout* _connectionButtonsLayout;

	BaseButton* _newProjectButton;
	BaseButton* _openProjectButton;
	BaseButton* _liveEditorButton;
};