#include "ProjectButton.h"

ProjectButton::ProjectButton(QWidget* parent)
	: BaseButton(parent), _projectName(""), _projectPath(""), _projectDescription("Project Description...")
{
}

void ProjectButton::SetProjectName(const QString& projectName)
{
	_projectName = projectName;
}

void ProjectButton::SetProjectPath(const QString& projectPath)
{
	_projectPath = projectPath;
}

void ProjectButton::SetProjectDescription(const QString& projectDescription)
{
	_projectDescription = projectDescription;
}

QString ProjectButton::GetProjectName() const
{
	return _projectName;
}

QString ProjectButton::GetProjectPath() const
{
	return _projectPath;
}

QString ProjectButton::GetProjectDescription() const
{
	return _projectDescription;
}

void ProjectButton::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(rect(), QColor(30, 30, 30));

    QFont nameFont = painter.font();
    nameFont.setPixelSize(16);
    painter.setFont(nameFont);

    QRect nameRect = QRect(0, 0, width(), height() / 2);
    QRect descRect = QRect(0, height() / 2, width(), height() / 2);

    // Disegna i testi
    painter.setPen(Qt::white);
    painter.drawText(nameRect, Qt::AlignCenter, _projectName);

    QFont descFont = painter.font();
    descFont.setPixelSize(12);
    painter.setFont(descFont);
    painter.drawText(descRect, Qt::AlignCenter, _projectDescription);
}
