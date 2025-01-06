#pragma once

#include "Engine/GUI/Elements/Elements.h"
#include "Engine/GUI/Windows/mainwindow.h"
#include "Engine/GUI/Text Fields/TextField.h"

#include <QLabel>
#include <QFrame>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileSystemModel>
#include <QTreeView>
#include <QTreeWidget>
#include <QListWidget>
#include <QSplitter>
#include <QObject>
#include <QMimeData>
#include <QSvgRenderer>
#include <QPainter>
#include <QMenu>
#include <QColorDialog>
#include <QSettings>
#include <QInputDialog>
#include <QDrag>

class IAssetsManager
{
public:
	virtual ~IAssetsManager() = default;

private:

};

class AssetsManager : public QFrame, public IAssetsManager, public IUIElement
{
	Q_OBJECT

public:
	AssetsManager(QWidget* parent = nullptr, const std::string& path = "");
	void SetProjectPath(const std::string& path);

private:
	QVBoxLayout* _container;
	QFileSystemModel* _model;
	QTreeView* _view;
	QHBoxLayout* _layout;
};

class ColorizedFolder
{
private:
	static QString _svgTemplate;

public:
	static void Initialize();

	static QIcon GetFolderIcon(const QColor& color);

	static QIcon SetFolderColor(QListWidgetItem* item, const QColor& color);
	static QIcon SetFolderColor(QTreeWidgetItem* item, const QColor& color);
};

class AssetTree : public QTreeWidget
{
	Q_OBJECT

public:
	AssetTree(QWidget* parent = nullptr);

	void SetProjectPath(const std::string& path);

protected:
	void dragEnterEvent(QDragEnterEvent* event) override;
	void dragMoveEvent(QDragMoveEvent* event) override;
	void dropEvent(QDropEvent* event) override;

signals:
	void filesChanged();

private:
	QString GetItemPath(QTreeWidgetItem* item);

	std::string _path;
};

class AssetViewer : public QListWidget
{
	Q_OBJECT
public:
	AssetViewer(QWidget* parent = nullptr);

	void SetProjectPath(const std::string& path);

protected:
	void startDrag(Qt::DropActions) override;
	void contextMenuEvent(QContextMenuEvent* event) override;
	void dragEnterEvent(QDragEnterEvent* event) override;
	void dragMoveEvent(QDragMoveEvent* event) override;
	void dropEvent(QDropEvent* event) override;

private:
	std::string _path;

signals:
	void filesChanged();
	void folderColorChanged(const QString& path, const QColor& color);
};

class AssetPanel : public QFrame, public IAssetsManager, public IUIElement
{
	Q_OBJECT

public:
	AssetPanel(QWidget* parent = nullptr, const std::string& path = "");
	void SetProjectPath(const std::string& path);

private slots:
	void onTreeItemSelected();
	void refreshViews();
	void saveFolderColor(const QString& path, const QColor& color);

private:
	void PopulateTree(const QDir& dir, QTreeWidgetItem* parentItem);
	QIcon GetIconForFile(const QFileInfo& info);
	QString GetItemPath(QTreeWidgetItem* item);
	QColor LoadFolderColor(const QString& path);

	void RefreshTreeColors(const QString& modifiedPath);
	void UpdateTreeItemColor(QTreeWidgetItem* item, const QString& modifiedPath);

	std::string _path;

	QVBoxLayout* _container;
	QSplitter* _splitter;
	AssetTree* _view;
	AssetViewer* _items;
	TextField* _search;
};