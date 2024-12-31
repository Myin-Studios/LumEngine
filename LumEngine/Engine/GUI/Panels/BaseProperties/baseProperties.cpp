#include "baseProperties.h"

Transform3DProperty::Transform3DProperty(QWidget* parent)
	: PropertyGroup("Transform3D", parent)
{
    const QStringList labels = { "Position", "Rotation", "Scale" };

    for (const auto& label : labels) {
        auto labelWidget = new QLabel(label, this);
        auto fieldWidget = new Vec3Property(this);

        transformationsLabels.append(labelWidget);
        transformationsFields.append(fieldWidget);

        // Usa il metodo della classe base che sa come gestire correttamente il layout
        addElement(labelWidget, Qt::AlignTop);
        addElement(fieldWidget, Qt::AlignTop);
    }

    SetupConnections();
}

void Transform3DProperty::SetEntity(BaseEntity* e)
{
    _entity = e;
    if (_entity) {
        UpdateValues();
    }
}

BaseEntity* Transform3DProperty::GetEntity()
{
    return _entity;
}

void Transform3DProperty::UpdateValues()
{
    if (!_entity) {
        qDebug() << "Entity is null";
        return;
    }

    auto transform = _entity->GetCoreProperty<Transform3DCore>();
    if (!transform) {
        qDebug() << "Transform3DCore not found";
        return;
    }

    auto pos = transform->GetPosition();
    auto rot = transform->GetRotation();
    auto scale = transform->GetScale();

    if (!transformationsFields[0] || !transformationsFields[1] || !transformationsFields[2]) {
        qDebug() << "One or more Vec3Property fields are null";
        return;
    }

    transformationsFields[0]->setValues(pos.x(), pos.y(), pos.z());
    transformationsFields[1]->setValues(rot.x(), rot.y(), rot.z());
    transformationsFields[2]->setValues(scale.x(), scale.y(), scale.z());
}

void Transform3DProperty::HandlePositionChanged(float x, float y, float z)
{
    if (this->_entity && this->_entity->GetCoreProperty<Transform3DCore>())
    {
        this->_entity->GetCoreProperty<Transform3DCore>()->SetPosition(x, y, z);

        if (auto renderer = RendererCore::GetInstance())
            renderer->update();
    }
}

void Transform3DProperty::HandleRotationChanged(float x, float y, float z)
{
    if (this->_entity && this->_entity->GetCoreProperty<Transform3DCore>())
    {
        this->_entity->GetCoreProperty<Transform3DCore>()->SetRotation(x, y, z);

        if (auto renderer = RendererCore::GetInstance())
            renderer->update();
    }
}

void Transform3DProperty::HandleScaleChanged(float x, float y, float z)
{
    if (this->_entity && this->_entity->GetCoreProperty<Transform3DCore>())
    {
        this->_entity->GetCoreProperty<Transform3DCore>()->SetScale(x, y, z);

        if (auto renderer = RendererCore::GetInstance())
            renderer->update();
    }
}

void Transform3DProperty::SetupConnections()
{
    connect(transformationsFields[0], &Vec3Property::valueChanged,
        this, &Transform3DProperty::HandlePositionChanged);
    connect(transformationsFields[1], &Vec3Property::valueChanged,
        this, &Transform3DProperty::HandleRotationChanged);
    connect(transformationsFields[2], &Vec3Property::valueChanged,
        this, &Transform3DProperty::HandleScaleChanged);
}

ColliderProperty::ColliderProperty(QWidget* parent) : PropertyGroup("Collider", parent)
{
	colliderLabels.append(new QLabel("Type", this));
	colliderLabels.append(new QLabel("Size", this));
    colliderField = new QComboBox(this);
	colliderField->addItems({ "Box", "Sphere", "Mesh"});
    colliderSize = new Vec3Property();

	addElement(colliderLabels[0], Qt::AlignTop);
	addElement(colliderField, Qt::AlignTop);
	addElement(colliderLabels[1], Qt::AlignTop);
	addElement(colliderSize, Qt::AlignTop);

	SetupConnections();
}

void ColliderProperty::SetEntity(BaseEntity* e)
{
	_entity = e;
	if (_entity) {
		UpdateValues();
	}
}

BaseEntity* ColliderProperty::GetEntity()
{
    return _entity;
}

void ColliderProperty::UpdateValues()
{
	if (!_entity) {
		return;
	}

	auto collider = _entity->GetCoreProperty<LumEngine::Physics::Collider>();
	if (!collider) {
		return;
	}

	auto type = collider->GetColliderType();
	auto size = collider->GetSize();

	colliderField->setCurrentIndex(static_cast<int>(type));
	colliderSize->setValues(size.x(), size.y(), size.z());
}

void ColliderProperty::HandleColliderTypeChanged(int index)
{
	if (this->_entity && this->_entity->GetCoreProperty<LumEngine::Physics::Collider>())
	{
		this->_entity->GetCoreProperty<LumEngine::Physics::Collider>()->SetColliderType(static_cast<ColliderType>(index));

		if (auto renderer = RendererCore::GetInstance())
			renderer->update();
	}
}

void ColliderProperty::HandleColliderSizeChanged(float x, float y, float z)
{
	if (this->_entity && this->_entity->GetCoreProperty<LumEngine::Physics::Collider>())
	{
		this->_entity->GetCoreProperty<LumEngine::Physics::Collider>()->SetSize(x, y, z);

		if (auto renderer = RendererCore::GetInstance())
			renderer->update();
	}
}

void ColliderProperty::SetupConnections()
{
	connect(colliderField, QOverload<int>::of(&QComboBox::currentIndexChanged),
		this, &ColliderProperty::HandleColliderTypeChanged);
	connect(colliderSize, &Vec3Property::valueChanged,
		this, &ColliderProperty::HandleColliderSizeChanged);
}

LODProperty::LODProperty(QWidget* parent) : PropertyGroup("LOD", parent)
{
	lodLabels.append(new QLabel("Steps", this));
	lodSteps = new NumberOperatorLineEdit();
	lodLabels.append(new QLabel("Max Distance", this));
	lodMaxDistance = new NumberOperatorLineEdit();
	lodLabels.append(new QLabel("Min Distance", this));
	lodMinDistance = new NumberOperatorLineEdit();

    lodSteps->setCoordinate(Coordinate::NEUTER);
    lodMaxDistance->setCoordinate(Coordinate::NEUTER);
    lodMinDistance->setCoordinate(Coordinate::NEUTER);

	addElement(lodLabels[0], Qt::AlignTop);
	addElement(lodSteps, Qt::AlignTop);
	addElement(lodLabels[1], Qt::AlignTop);
	addElement(lodMaxDistance, Qt::AlignTop);
	addElement(lodLabels[2], Qt::AlignTop);
	addElement(lodMinDistance, Qt::AlignTop);

	SetupConnections();
}

void LODProperty::SetEntity(BaseEntity* e)
{
	this->_entity = e;
	if (this->_entity) {
		UpdateValues();
	}
}

BaseEntity* LODProperty::GetEntity()
{
	return this->_entity;
}

void LODProperty::UpdateValues()
{
	if (!this->_entity) {
		return;
	}

	auto lod = this->_entity->GetCoreProperty<LumEngine::Graphics::LODCore>();
	if (!lod) {
		return;
	}

	auto steps = lod->GetSteps();
	auto maxDistance = lod->GetMaxDistance();
	auto minDistance = lod->GetMinDistance();

	lodSteps->setText(QString::number(steps));
	lodMaxDistance->setText(QString::number(maxDistance));
	lodMinDistance->setText(QString::number(minDistance));
}

void LODProperty::HandleLODStepsChanged(QString value)
{
	if (this->_entity && this->_entity->GetCoreProperty<LumEngine::Graphics::LODCore>())
	{
		this->_entity->GetCoreProperty<LumEngine::Graphics::LODCore>()->SetSteps(value.toFloat());

		if (auto renderer = RendererCore::GetInstance())
			renderer->update();
	}
}

void LODProperty::HandleLODMaxDistanceChanged(QString value)
{
	if (this->_entity && this->_entity->GetCoreProperty<LumEngine::Graphics::LODCore>())
	{
		this->_entity->GetCoreProperty<LumEngine::Graphics::LODCore>()->SetMaxDistance(value.toFloat());

		if (auto renderer = RendererCore::GetInstance())
			renderer->update();
	}
}

void LODProperty::HandleLODMinDistanceChanged(QString value)
{
	if (this->_entity && this->_entity->GetCoreProperty<LumEngine::Graphics::LODCore>())
	{
		this->_entity->GetCoreProperty<LumEngine::Graphics::LODCore>()->SetMinDistance(value.toFloat());

		if (auto renderer = RendererCore::GetInstance())
			renderer->update();
	}
}

void LODProperty::SetupConnections()
{
	connect(lodSteps, &NumberOperatorLineEdit::textChanged,
		this, &LODProperty::HandleLODStepsChanged);
	connect(lodMaxDistance, &NumberOperatorLineEdit::textChanged,
		this, &LODProperty::HandleLODMaxDistanceChanged);
	connect(lodMinDistance, &NumberOperatorLineEdit::textChanged,
		this, &LODProperty::HandleLODMinDistanceChanged);
}
