//
// Created by darkd on 12/07/2024.
//

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    Camera() : _position(0.0, 0.0, 3.0),
    _right(glm::vec3(1.0, 0.0, 0.0)),
    _up(glm::vec3(0.0, 1.0, 0.0)),
    _front(glm::vec3(0.0, 0.0, 1.0)),
    _yaw(-90.0f), _pitch(0.0f), _roll(0.0f)
    {

    }

    void move(glm::vec3 offset)
    {
        _position += offset;
    }

    void rotate(float yaw, float pitch, float roll)
    {
        _yaw += yaw;
        _pitch += pitch;
        _roll += roll;

        if (_pitch > 89.0f) _pitch = 89.0f;
        if (_pitch < -89.0f) _pitch = -89.0f;

        glm::vec3 front;
        front.x = glm::cos(glm::radians(_yaw)) * glm::cos(glm::radians(_pitch));
        front.y = glm::sin(glm::radians(_pitch));
        front.z = glm::sin(glm::radians(_yaw)) * glm::cos(glm::radians(_pitch));

        _front = glm::normalize(front);
        _right = glm::normalize(glm::cross(_front, glm::vec3(0.0f, 1.0f, 0.0f)));
        _up = glm::normalize(glm::cross(_right, _front));
    }

    glm::vec3 getPos() const
    {
        return _position;
    }

    glm::mat4 getViewMat() const
    {
        return glm::lookAt(_position, _position + _front, _up);
    }

private:
    glm::vec3 _position;

    glm::vec3 _right;
    glm::vec3 _up;
    glm::vec3 _front;

    float _yaw;
    float _pitch;
    float _roll;
};

#endif //CAMERA_H
