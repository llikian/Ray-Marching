/***************************************************************************************************
 * @file  Camera.cpp
 * @brief Implementation of the Camera class
 **************************************************************************************************/

#include "Camera.hpp"

#include <cmath>
#include "maths/geometry.hpp"
#include "maths/trigonometry.hpp"

Camera::Camera(const Point& position)
    : position(position),
      worldUp(0.0f, 1.0f, 0.0f),
      view(1.0f) {

    const Vector direction = -1.0f * normalize(position);
    pitch = asinf(direction.y);
    yaw = asinf(direction.z / cosf(pitch));

    look(vec2());
}

Matrix4 Camera::getVPmatrix(const Matrix4& projection) {
    return projection * view;
}

Point Camera::getPosition() const {
    return position;
}

Point Camera::getDirection() const {
    return front;
}

Point Camera::getRight() const {
    return right;
}

Point Camera::getUp() const {
    return up;
}

void Camera::move(CameraControls direction, float deltaTime) {
    const float speed = 5.0f * deltaTime;

    switch(direction) {
        case CameraControls::forward:
            position += front * speed;
            break;
        case CameraControls::backward:
            position -= front * speed;
            break;
        case CameraControls::left:
            position -= normalize(cross(front, worldUp)) * speed;
            break;
        case CameraControls::right:
            position += normalize(cross(front, worldUp)) * speed;
            break;
        case CameraControls::upward:
            position.y += speed;
            break;
        case CameraControls::downward:
            position.y -= speed;
            break;
    }

    view[0][3] = -dot(right, position);
    view[1][3] = -dot(up, position);
    view[2][3] = dot(front, position);
}

void Camera::look(vec2 mouseOffset) {
    constexpr float sensitivity = 0.1f;
    constexpr float epsilon = 0.00001f;
    mouseOffset *= sensitivity;

    yaw += radians(mouseOffset.x);
    if(yaw > 2.0f * M_PIf) {
        yaw -= 2.0f * M_PIf;
    } else if(yaw < -2.0f * M_PIf) {
        yaw += 2.0f * M_PIf;
    }

    pitch -= radians(mouseOffset.y);
    if(pitch > M_PI_2f - epsilon) {
        pitch = M_PI_2f - epsilon;
    } else if(pitch < -M_PI_2f + epsilon) {
        pitch = -M_PI_2f + epsilon;
    }

    front.x = cosf(pitch) * cosf(yaw);
    front.y = sinf(pitch);
    front.z = cosf(pitch) * sinf(yaw);

    right = normalize(cross(front, worldUp));
    up = normalize(cross(right, front));
    
    view[0][0] = right.x;
    view[0][1] = right.y;
    view[0][2] = right.z;
    view[0][3] = -dot(right, position);
    
    view[1][0] = up.x;
    view[1][1] = up.y;
    view[1][2] = up.z;
    view[1][3] = -dot(up, position);
    
    view[2][0] = -front.x;
    view[2][1] = -front.y;
    view[2][2] = -front.z;
    view[2][3] = dot(front, position);
}
