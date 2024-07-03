#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix4x4>

class Camera {
public:
    Camera();

    void setPosition(const QVector3D& position);
    void setRotation(float yaw, float pitch, float roll);
    void move(const QVector3D& offset);
    void rotate(float yaw, float pitch, float roll);
    QMatrix4x4 getViewMatrix() const;

    QVector3D front() const { return m_front; }
    QVector3D up() const { return m_up; }
    QVector3D right() const { return m_right; }
    QVector3D position() const { return m_position; }

private:
    QVector3D m_position;
    QVector3D m_front;
    QVector3D m_up;
    QVector3D m_right;
    float m_yaw;
    float m_pitch;
    float m_roll;
};


#endif // CAMERA_H
