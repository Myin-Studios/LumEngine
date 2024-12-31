#pragma once

#include <Engine/GUI/Panels/properties.h>
#include "../LumTypes/Entities/Entity.h"
#include "Engine/GUI/Rendering/renderer.h"
#include "../LumTypes/Physics/Collisions.h"
#include "../LumTypes/LOD/LOD.h"

#include <QObject>
#include <QWidget>
#include <QList>
#include <QLabel>
#include <QComboBox>

#include <iostream>

class Transform3DProperty : public PropertyGroup
{
	Q_OBJECT
public:
	Transform3DProperty(QWidget* parent = nullptr);

	void SetEntity(BaseEntity* e);
	BaseEntity* GetEntity() override;

	void UpdateValues() override;

private slots:
	void HandlePositionChanged(float x, float y, float z);
	void HandleRotationChanged(float x, float y, float z);
	void HandleScaleChanged(float x, float y, float z);

private:
	void SetupConnections();

	BaseEntity* _entity = nullptr;

	QList<QLabel*> transformationsLabels;
	QList<Vec3Property*> transformationsFields;
};

class ColliderProperty : public PropertyGroup
{
	Q_OBJECT

public:
	ColliderProperty(QWidget* parent = nullptr);

	void SetEntity(BaseEntity* e);
	BaseEntity* GetEntity() override;

	void UpdateValues() override;

private slots:
	void HandleColliderTypeChanged(int index);
	void HandleColliderSizeChanged(float x, float y, float z);

private:
	void SetupConnections();

	BaseEntity* _entity = nullptr;

	QList<QLabel*> colliderLabels;
	QComboBox* colliderField;
	Vec3Property* colliderSize;
};

class LODProperty : public PropertyGroup
{
	Q_OBJECT
public:
	LODProperty(QWidget* parent = nullptr);

	void SetEntity(BaseEntity* e);
	BaseEntity* GetEntity() override;

	void UpdateValues() override;

private slots:
	void HandleLODStepsChanged(QString value);
	void HandleLODMaxDistanceChanged(QString value);
	void HandleLODMinDistanceChanged(QString value);

private:
	void SetupConnections();

	BaseEntity* _entity = nullptr;

	QList<QLabel*> lodLabels;
	NumberOperatorLineEdit* lodSteps;
	NumberOperatorLineEdit* lodMaxDistance;
	NumberOperatorLineEdit* lodMinDistance;
};