#pragma once

#include "Engine/GUI/Loading/loadingWindow.h"
#include "Engine/GUI/guibuilder.h"
#include "Engine/Core/hostInitializer.h"

#include <QMainWindow>
#include <QVBoxLayout>
#include "QCoreApplication"
#include "QStyleFactory"
#include <QMetaObject>
#include <QSplitter>
#include <QFrame>
#include <QObject>

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

public slots:
	void onNewProjectClicked();
	void onOpenProjectClicked();

private:
	void RunEditor() override;

	LoadingWindow* _loadingWindow;
	
	std::shared_ptr<MainWindow> _mainWindow;
	std::shared_ptr<LoadingThread> _loadingThread;
	std::shared_ptr<GuiBuilder> _builder;
};

class HomeBuilder
{
public:
	HomeBuilder();

private:
	ProjectManager* _projectManager;

	std::shared_ptr<MainWindow> _window;

	LoadingThread* _loadingThread;

	VerticalPanel* _leftPanel;

	QSplitter* _mainSplitter;

	QFrame* _mainContainer;

	QFrame* _buttonsContainer;

	QHBoxLayout* _mainButtonsLayout;
	QHBoxLayout* _projectButtonsLayout;
	QHBoxLayout* _connectionButtonsLayout;

	BaseButton* _newProjectButton;
	BaseButton* _openProjectButton;
	BaseButton* _connectToButton;
	BaseButton* _liveEditorButton;
};