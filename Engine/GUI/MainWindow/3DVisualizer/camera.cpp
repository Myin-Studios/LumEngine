#include "camera.h"

Camera::Camera()
        : m_position(QVector3D(0.0f, 0.0f, 3.0f)),
          m_front(QVector3D(0.0f, 0.0f, -1.0f)),
          m_up(QVector3D(0.0f, 1.0f, 0.0f)),
          m_right(QVector3D(1.0f, 0.0f, 0.0f)),
          m_yaw(-90.0f),
          m_pitch(0.0f),
          m_roll(0.0f)
{
}

void Camera::setPosition(const QVector3D& position)
{
    m_position = position;
}

void Camera::setRotation(float yaw, float pitch, float roll)
{
    m_yaw = yaw;
    m_pitch = pitch;
    m_roll = roll;
}

void Camera::move(const QVector3D& offset)
{
    m_position += offset;
}

void Camera::rotate(float yaw, float pitch, float roll)
{
    m_yaw += yaw;
    m_pitch += pitch;
    m_roll += roll;

    // Limit pitch to avoid gimbal lock
    if (m_pitch > 89.0f) m_pitch = 89.0f;
    if (m_pitch < -89.0f) m_pitch = -89.0f;

    // Update front, right and up vectors based on yaw, pitch and roll
    QVector3D front;
    front.setX(cos(qDegreesToRadians(m_yaw)) * cos(qDegreesToRadians(m_pitch)));
    front.setY(sin(qDegreesToRadians(m_pitch)));
    front.setZ(sin(qDegreesToRadians(m_yaw)) * cos(qDegreesToRadians(m_pitch)));
    m_front = front.normalized();

    m_right = QVector3D::crossProduct(m_front, QVector3D(0.0f, 1.0f, 0.0f)).normalized();
    m_up = QVector3D::crossProduct(m_right, m_front).normalized();
}

QMatrix4x4 Camera::getViewMatrix() const
{
    QMatrix4x4 view;
    view.lookAt(m_position, m_position + m_front, m_up);
    return view;
}

