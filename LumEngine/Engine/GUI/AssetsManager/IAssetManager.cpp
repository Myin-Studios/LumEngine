#include "IAssetManager.h"

AssetsManager::AssetsManager(QWidget* parent, const std::string& path) : QFrame(parent)
{
	_container = new QVBoxLayout(this);
	_model = new QFileSystemModel(this);
	_view = new QTreeView(this);
	_layout = new QHBoxLayout(this);

	_model->setRootPath(QString::fromStdString(path));
	_model->setFilter(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);

	_view->setModel(_model);
	_view->setRootIndex(_model->index(QString::fromStdString(path)));
	_view->setAnimated(true);
	_view->setIndentation(20);
	_view->setSortingEnabled(true);
	_view->setAcceptDrops(true);
	_view->setDragEnabled(true);
	_view->setDragDropMode(QAbstractItemView::InternalMove);
	_view->setDropIndicatorShown(true);
	_view->setSelectionMode(QAbstractItemView::ExtendedSelection);
	_view->setSelectionBehavior(QAbstractItemView::SelectItems);
	_view->setEditTriggers(QAbstractItemView::EditKeyPressed | QAbstractItemView::SelectedClicked);

	_container->addLayout(_layout);

	_layout->addWidget(_view);

	setLayout(_container);
}

void AssetsManager::SetProjectPath(const std::string& path)
{
	_model->setRootPath(QString::fromStdString(path));
	_view->setRootIndex(_model->index(QString::fromStdString(path)));
}

AssetPanel::AssetPanel(QWidget* parent, const std::string& path) : QFrame(parent)
{
	ColorizedFolder::Initialize();

	_container = new QVBoxLayout(this);
	_splitter = new QSplitter(this);
	_view = new AssetTree(this);
	_items = new AssetViewer(this);
	_search = new TextField(this);

	_container->addWidget(_search);
	_container->addWidget(_splitter);

	_splitter->addWidget(_view);
	_splitter->addWidget(_items);

	_view->setColumnCount(2);
	_view->setHeaderLabels({ "Name", "Type" });
	_view->setColumnWidth(0, 200);
	_view->setColumnWidth(1, 100);
	_view->setSelectionMode(QAbstractItemView::ExtendedSelection);
	_view->setSelectionBehavior(QAbstractItemView::SelectRows);
	_view->setDragEnabled(true);
	_view->setDragDropMode(QAbstractItemView::DragDrop);
	_view->setDropIndicatorShown(true);
	_view->setAcceptDrops(true);
	_view->setEditTriggers(QAbstractItemView::EditKeyPressed | QAbstractItemView::SelectedClicked);

	_items->setWrapping(true);
	_items->setFlow(QListView::LeftToRight);
	_items->setViewMode(QListView::IconMode);
	_items->setResizeMode(QListView::Adjust);
	_items->setGridSize(QSize(128, 128));
	_items->setIconSize(QSize(64, 64));
	_items->setSpacing(10);
	_items->setUniformItemSizes(true);
	_items->setMovement(QListView::Static);
	_items->setDragEnabled(true);
	_items->setAcceptDrops(true);
	_items->setDropIndicatorShown(true);
	_items->setDragDropMode(QAbstractItemView::DragDrop);

	_search->setPlaceholderText("Search asset...");

	setLayout(_container);

	connect(_view, &QTreeWidget::itemSelectionChanged, this, &AssetPanel::onTreeItemSelected);
	connect(_view, &AssetTree::filesChanged, this, &AssetPanel::refreshViews);
	connect(_items, &AssetViewer::filesChanged, this, &AssetPanel::refreshViews);
	connect(_items, &AssetViewer::folderColorChanged, this, &AssetPanel::saveFolderColor);
	connect(_items, &AssetViewer::folderColorChanged,
		this, [this](const QString& path, const QColor& color) {
			RefreshTreeColors(path);
		});
}

void AssetPanel::SetProjectPath(const std::string& path)
{
	_path = path;
	_view->clear();  // Pulisci la vista prima di ricaricare

	QDir dir(QString::fromStdString(path));
	QTreeWidgetItem* root = new QTreeWidgetItem(_view);
	root->setText(0, dir.dirName());
	_view->addTopLevelItem(root);

	_view->SetProjectPath(path);
	_items->SetProjectPath(path);

	PopulateTree(dir, root);
}

void AssetPanel::refreshViews()
{
	// Aggiorna entrambe le viste
	PopulateTree(QDir(QString::fromStdString(_path)), nullptr);
	onTreeItemSelected();  // Aggiorna la vista icone
}

void AssetPanel::PopulateTree(const QDir& dir, QTreeWidgetItem* parentItem)
{
	QFileInfoList entries = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);

	if (parentItem && parentItem->treeWidget()) {
		parentItem->treeWidget()->setIconSize(QSize(16, 16));
	}

	for (const QFileInfo& info : entries) {
		QTreeWidgetItem* item = new QTreeWidgetItem(parentItem);
		item->setText(0, info.fileName());

		// Se è una directory, esplora ricorsivamente
		if (info.isDir()) {
			item->setText(1, "Folder");

			// Carica il colore salvato o usa il default
			QColor color = LoadFolderColor(info.filePath());  // Implementa questa funzione
			item->setIcon(0, ColorizedFolder::GetFolderIcon(color));
			item->setData(0, Qt::UserRole, color);

			QDir subDir(info.filePath());
			LoadFolderColor(info.filePath());
			PopulateTree(subDir, item);
		}
		else {
			// Per i file, mostra l'estensione o il tipo
			QString ext = info.suffix().toLower();
			item->setText(1, ext);

			// Opzionale: imposta icone diverse in base al tipo di file
			if (ext == "png" || ext == "jpg" || ext == "jpeg") {
				// item->setIcon(0, QIcon("path/to/image/icon.png"));
			}
			// ... altri tipi di file ...
		}
	}
}

void AssetPanel::onTreeItemSelected()
{
	_items->clear();

	QTreeWidgetItem* selectedItem = _view->currentItem();
	if (!selectedItem) {
		qDebug() << "No item selected";
		return;
	}

	QString path = GetItemPath(selectedItem);
	qDebug() << "Selected path:" << path;

	QDir dir(path);
	if (!dir.exists()) {
		qDebug() << "Directory doesn't exist:" << path;
		return;
	}

	QFileInfoList entries = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
	qDebug() << "Found" << entries.size() << "entries";

	for (const QFileInfo& info : entries) {
		QListWidgetItem* item = new QListWidgetItem();

		item->setText(info.fileName());
		item->setTextAlignment(Qt::AlignCenter);

		QIcon icon = GetIconForFile(info);
		if (icon.isNull()) {
			qDebug() << "Icon is null for" << info.fileName();
		}
		item->setIcon(icon);

		_items->addItem(item);

		if (info.isDir()) {
			QColor color = LoadFolderColor(info.filePath());
			ColorizedFolder::SetFolderColor(item, color);
		}
		else {
			item->setIcon(GetIconForFile(info));
		}

		qDebug() << "Added item:" << info.fileName();
	}
}

QIcon AssetPanel::GetIconForFile(const QFileInfo& info) {
	QString ext = info.suffix().toLower();
	if (info.isDir())
		return QIcon("Resources/Icons/AssetIcons/FolderAsset.svg");
	else if (ext == "png" || ext == "jpg" || ext == "jpeg")
		return QIcon("Resources/Icons/AssetIcons/ImageAsset.svg");
	else if (ext == "obj" || ext == "fbx")
		return QIcon("Resources/Icons/AssetIcons/ModelAsset.svg");
	else if (ext == "cs")
		return QIcon("Resources/Icons/AssetIcons/CodeAsset.svg");
	return QIcon("Resources/Icons/AssetIcons/DefaultAsset.svg");
}

QString AssetPanel::GetItemPath(QTreeWidgetItem* item)
{
	QStringList path;
	while (item) {
		path.prepend(item->text(0));
		item = item->parent();
	}

	// Rimuovi il primo elemento poiché è già incluso in _path
	if (!path.isEmpty()) {
		path.removeFirst();
	}

	return QString::fromStdString(_path) + "/" + path.join("/");
}

void AssetPanel::saveFolderColor(const QString& path, const QColor& color)
{
	QSettings settings("Myin Studios", "LumE");
	settings.setValue("FolderColors/" + path, color.name());
}

QColor AssetPanel::LoadFolderColor(const QString& path)
{
	QSettings settings("Myin Studios", "LumE");
	QString colorName = settings.value("FolderColors/" + path, "#FFFFFF").toString();
	return QColor(colorName);
}

void AssetPanel::RefreshTreeColors(const QString& modifiedPath) {
	// Trova l'item nella tree che corrisponde al path modificato e aggiorna il suo colore
	for (int i = 0; i < _view->topLevelItemCount(); ++i) {
		UpdateTreeItemColor(_view->topLevelItem(i), modifiedPath);
	}
}

void AssetPanel::UpdateTreeItemColor(QTreeWidgetItem* item, const QString& modifiedPath) {
	QString itemPath = GetItemPath(item);

	if (itemPath == modifiedPath) {
		QColor color = LoadFolderColor(itemPath);
		ColorizedFolder::SetFolderColor(item, color);
	}

	// Controlla ricorsivamente i figli
	for (int i = 0; i < item->childCount(); ++i) {
		UpdateTreeItemColor(item->child(i), modifiedPath);
	}
}

AssetTree::AssetTree(QWidget* parent) : QTreeWidget(parent)
{
	setAcceptDrops(true);
}

void AssetTree::SetProjectPath(const std::string& path)
{
	_path = path;
}

void AssetTree::dragEnterEvent(QDragEnterEvent* event)
{
	if (event->mimeData()->hasUrls() || event->source()) {
		event->acceptProposedAction();
	}
}

void AssetTree::dragMoveEvent(QDragMoveEvent* event)
{
	event->acceptProposedAction();
}

void AssetTree::dropEvent(QDropEvent* event)
{
	if (event->mimeData()->hasUrls()) {
		// File esterni - Copia
		for (const QUrl& url : event->mimeData()->urls()) {
			QString srcPath = url.toLocalFile();
			QFileInfo fileInfo(srcPath);

			// Ottieni la directory di destinazione
			QTreeWidgetItem* dropItem = itemAt(event->position().toPoint());
			QString destDir = dropItem ? GetItemPath(dropItem) : QString::fromStdString(_path);
			QString destPath = QDir(destDir).filePath(fileInfo.fileName());

			qDebug() << "Source path:" << srcPath;
			qDebug() << "Destination path:" << destPath;

			// Verifica che la directory di destinazione esista
			QDir dir(destDir);
			if (!dir.exists()) {
				if (!dir.mkpath(".")) {
					QMessageBox::warning(this, "Error", "Failed to create directory: " + destDir);
					continue;
				}
			}

			// Prova a copiare il file
			QFile sourceFile(srcPath);
			if (!sourceFile.copy(destPath)) {
				QMessageBox::warning(this, "Error",
					QString("Failed to copy file: %1\nError: %2")
					.arg(fileInfo.fileName())
					.arg(sourceFile.errorString()));
				continue;
			}

			// Verifica che il file sia stato effettivamente copiato
			if (!QFile::exists(destPath)) {
				QMessageBox::warning(this, "Error",
					"File was not copied successfully: " + fileInfo.fileName());
				continue;
			}
		}
		event->acceptProposedAction();
		emit filesChanged();
	}
	else if (event->source()) {
		// File interni - Sposta
		QTreeWidgetItem* sourceItem = currentItem();
		QTreeWidgetItem* targetItem = itemAt(event->position().toPoint());

		if (sourceItem && targetItem) {
			QString sourcePath = GetItemPath(sourceItem);
			QString targetDir = GetItemPath(targetItem);
			QString fileName = sourceItem->text(0);
			QString newPath = QDir(targetDir).filePath(fileName);

			qDebug() << "Moving from:" << sourcePath;
			qDebug() << "Moving to:" << newPath;

			if (QFile::rename(sourcePath, newPath)) {
				QTreeWidget::dropEvent(event);
				emit filesChanged();
			}
			else {
				QMessageBox::warning(this, "Error", "Failed to move file: " + fileName);
			}
		}
	}
}

QString AssetTree::GetItemPath(QTreeWidgetItem* item)
{
	QStringList path;
	while (item) {
		path.prepend(item->text(0));
		item = item->parent();
	}

	// Rimuovi il primo elemento poiché è già incluso in _path
	if (!path.isEmpty()) {
		path.removeFirst();
	}

	return QString::fromStdString(_path) + "/" + path.join("/");
}

AssetViewer::AssetViewer(QWidget* parent) : QListWidget(parent)
{
	setAcceptDrops(true);
}

void AssetViewer::SetProjectPath(const std::string& path)
{
	_path = path;
}

void AssetViewer::startDrag(Qt::DropActions)
{
	QListWidgetItem* item = currentItem();
	if (!item) return;

	QString filePath = QDir(QString::fromStdString(_path)).filePath(item->text());

	QMimeData* mimeData = new QMimeData;
	mimeData->setText(filePath);
	mimeData->setData("application/x-lume-asset", filePath.toUtf8());

	QDrag* drag = new QDrag(this);
	drag->setMimeData(mimeData);

	drag->setPixmap(item->icon().pixmap(32, 32));

	drag->exec(Qt::CopyAction);
}

void AssetViewer::contextMenuEvent(QContextMenuEvent* event)
{
	QListWidgetItem* item = itemAt(event->pos());
	QMenu menu;

	if (!item)
	{
		menu.addAction("New Folder", [=]() {
			bool ok;
			QString folderName = QInputDialog::getText(this, "New Folder", "Folder Name:", QLineEdit::Normal, "", &ok);
			if (ok && !folderName.isEmpty()) {
				QDir dir(QString::fromStdString(_path));
				if (dir.mkdir(folderName)) {
					emit filesChanged();
				}
				else {
					QMessageBox::warning(this, "Error", "Failed to create folder");
				}
			}
			});
		menu.exec(event->globalPos());
		return;
	}

	// Ottieni il path completo dell'item
	QString itemPath = QDir(QString::fromStdString(_path)).filePath(item->text());
	QFileInfo fileInfo(itemPath);

	if (fileInfo.isDir()) {  // Controlla se è una directory
		menu.addSection("Folder Colors");

		menu.addAction("Default Color", [=]() {
			ColorizedFolder::SetFolderColor(item, QColor("#FFFFFF"));
			emit folderColorChanged(itemPath, QColor("#FFFFFF"));
			});

		menu.addAction("Red", [=]() {
			ColorizedFolder::SetFolderColor(item, QColor("#FF0000"));
			emit folderColorChanged(itemPath, QColor("#FF0000"));
			});

		menu.addAction("Green", [=]() {
			ColorizedFolder::SetFolderColor(item, QColor("#00FF00"));
			emit folderColorChanged(itemPath, QColor("#00FF00"));
			});

		menu.addAction("Blue", [=]() {
			ColorizedFolder::SetFolderColor(item, QColor("#0000FF"));
			emit folderColorChanged(itemPath, QColor("#0000FF"));
			});

		menu.addSeparator();
		menu.addAction("Custom Color...", [=]() {
			QColor color = QColorDialog::getColor(Qt::white, this);
			if (color.isValid()) {
				ColorizedFolder::SetFolderColor(item, color);
				emit folderColorChanged(itemPath, color);
			}
			});
	}

	QPoint pos = this->mapToGlobal(event->pos());
	menu.exec(pos);
}

void AssetViewer::dragEnterEvent(QDragEnterEvent* event)
{
	if (event->mimeData()->hasUrls() || event->source()) {
		event->acceptProposedAction();
	}
}

void AssetViewer::dragMoveEvent(QDragMoveEvent* event)
{
	event->acceptProposedAction();
}

void AssetViewer::dropEvent(QDropEvent* event)
{
	if (event->mimeData()->hasUrls()) {
		// File esterni - Copia
		for (const QUrl& url : event->mimeData()->urls()) {
			QString srcPath = url.toLocalFile();
			QFileInfo fileInfo(srcPath);
			QString destPath = QDir(QString::fromStdString(_path))
				.filePath(fileInfo.fileName());

			qDebug() << "Source path:" << srcPath;
			qDebug() << "Destination path:" << destPath;

			QDir dir(QString::fromStdString(_path));
			if (!dir.exists()) {
				if (!dir.mkpath(".")) {
					QMessageBox::warning(this, "Error", "Failed to create directory");
					continue;
				}
			}

			QFile sourceFile(srcPath);
			if (!sourceFile.copy(destPath)) {
				QMessageBox::warning(this, "Error",
					QString("Failed to copy file: %1\nError: %2")
					.arg(fileInfo.fileName())
					.arg(sourceFile.errorString()));
				continue;
			}

			if (!QFile::exists(destPath)) {
				QMessageBox::warning(this, "Error",
					"File was not copied successfully: " + fileInfo.fileName());
				continue;
			}
		}
		event->acceptProposedAction();
		emit filesChanged();
	}
	else if (event->source()) {
		// File interni - Sposta
		QListWidgetItem* item = currentItem();
		if (item) {
			QString sourcePath = QDir(QString::fromStdString(_path))
				.filePath(item->text());
			QString targetPath = QDir(QString::fromStdString(_path))
				.filePath(item->text());

			qDebug() << "Moving from:" << sourcePath;
			qDebug() << "Moving to:" << targetPath;

			if (sourcePath != targetPath) {
				if (QFile::rename(sourcePath, targetPath)) {
					QListWidget::dropEvent(event);
					emit filesChanged();
				}
				else {
					QMessageBox::warning(this, "Error", "Failed to move file: " + item->text());
				}
			}
		}
	}
}

QString ColorizedFolder::_svgTemplate;

void ColorizedFolder::Initialize()
{
	// Template SVG con placeholder per il colore
	_svgTemplate = R"(
            <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24">
                <path fill="%1" d="M10 4H4c-1.1 0-1.99.9-1.99 2L2 18c0 1.1.9 2 2 2h16c1.1 0 2-.9 2-2V8c0-1.1-.9-2-2-2h-8l-2-2z"/>
            </svg>
        )";
}

QIcon ColorizedFolder::GetFolderIcon(const QColor& color)
{
	// Crea SVG colorato
	QString svgContent = _svgTemplate.arg(color.name());

	// Converti SVG in QIcon
	QSvgRenderer renderer(svgContent.toUtf8());
	QPixmap pixmap(64, 64);  // dimensione desiderata
	pixmap.fill(Qt::transparent);
	QPainter painter(&pixmap);
	renderer.render(&painter);

	return QIcon(pixmap);
}

QIcon ColorizedFolder::SetFolderColor(QListWidgetItem* item, const QColor& color) {
	QString iconPath = "Resources/Icons/AssetIcons/FolderAsset.svg";

	QFile file(iconPath);
	if (!file.open(QIODevice::ReadOnly)) {
		qDebug() << "Failed to open SVG file";
		return item->icon();
	}

	QString svgContent = file.readAll();
	svgContent.replace("fill:white", QString("fill:%1").arg(color.name()));

	QSize targetSize = item->listWidget()->iconSize();
	QSvgRenderer renderer(svgContent.toUtf8());

	// Calcola le dimensioni mantenendo l'aspect ratio
	QSizeF originalSize = renderer.defaultSize();
	QSize scaledSize = originalSize.toSize().scaled(targetSize, Qt::KeepAspectRatio);

	QPixmap pixmap(scaledSize);
	pixmap.fill(Qt::transparent);
	QPainter painter(&pixmap);
	renderer.render(&painter);

	QIcon newIcon(pixmap);
	item->setIcon(newIcon);
	return newIcon;
}

QIcon ColorizedFolder::SetFolderColor(QTreeWidgetItem* item, const QColor& color) {
	QString iconPath = "Resources/Icons/AssetIcons/FolderAsset.svg";
	
	QFile file(iconPath);
	if (!file.open(QIODevice::ReadOnly)) {
		qDebug() << "Failed to open SVG file";
		return item->icon(0);
	}

	QString svgContent = file.readAll();
	svgContent.replace("fill:white", QString("fill:%1").arg(color.name()));

	QSize targetSize = item->treeWidget()->iconSize();
	QSvgRenderer renderer(svgContent.toUtf8());

	// Calcola le dimensioni mantenendo l'aspect ratio
	QSizeF originalSize = renderer.defaultSize();
	QSize scaledSize = originalSize.toSize().scaled(targetSize, Qt::KeepAspectRatio);

	QPixmap pixmap(scaledSize);
	pixmap.fill(Qt::transparent);
	QPainter painter(&pixmap);
	renderer.render(&painter);

	QIcon newIcon(pixmap);
	item->setIcon(0, newIcon);
	return newIcon;
}