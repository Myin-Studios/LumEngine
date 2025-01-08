#pragma once

#include "Engine/GUI/Buttons/basebutton.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QPainter>

class ProjectButton : public BaseButton
{
	Q_OBJECT

public:

	ProjectButton(QWidget* parent = nullptr);
	~ProjectButton() = default;

	void SetProjectName(const QString& projectName);
	void SetProjectPath(const QString& projectPath);
	void SetProjectDescription(const QString& projectDescription);

	QString GetProjectName() const;
	QString GetProjectPath() const;
	QString GetProjectDescription() const;

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	QString _projectName;
	QString _projectPath;
	QString _projectDescription;

	QLabel* _projectNameLabel;
	QLabel* _projectDescriptionLabel;

	QVBoxLayout* _layout;
};