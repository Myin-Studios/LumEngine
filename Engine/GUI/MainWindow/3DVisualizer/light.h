#ifndef LIGHT_H
#define LIGHT_H

#include <QVector3D>
#include <QColor>

struct Light {
    QVector3D position;
    QColor color;
    float intensity;
};

#endif // LIGHT_H
