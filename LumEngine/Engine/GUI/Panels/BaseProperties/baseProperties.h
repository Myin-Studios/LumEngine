#pragma once

#include <Engine/GUI/Panels/properties.h>

#include <QWidget>
#include <QList>
#include <QLabel>

#include <iostream>

class Transform3DProperty : public PropertyGroup
{
public:
	Transform3DProperty(QWidget* parent = nullptr);

private:
	QList<QLabel*> transformationsLabels;
	QList<Vec3Property*> transformationsFields;
};