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
}