/***************************************************************************************************
 * @file  Image.cpp
 * @brief Implementation of the Image class
 **************************************************************************************************/

#include "maths/transformations.hpp"

#include <cmath>
#include "maths/geometry.hpp"
#include "maths/trigonometry.hpp"

Matrix4 scale(float scalar) {
    return Matrix4(
        scalar, 0.0f, 0.0f,
        0.0f, scalar, 0.0f,
        0.0f, 0.0f, scalar
    );
}

Matrix4 scale(float x, float y, float z) {
    return Matrix4(
        x, 0.0f, 0.0f,
        0.0f, y, 0.0f,
        0.0f, 0.0f, z
    );
}

Matrix4 scaleX(float scalar) {
    return Matrix4(
        scalar, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    );
}

Matrix4 scaleY(float scalar) {
    return Matrix4(
        1.0f, 0.0f, 0.0f,
        0.0f, scalar, 0.0f,
        0.0f, 0.0f, 1.0f
    );
}

Matrix4 scaleZ(float scalar) {
    return Matrix4(
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, scalar
    );
}

Matrix4 translate(const Vector& vector) {
    return Matrix4(
        1.0f, 0.0f, 0.0f, vector.x,
        0.0f, 1.0f, 0.0f, vector.y,
        0.0f, 0.0f, 1.0f, vector.z,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}

Matrix4 translate(float x, float y, float z) {
    return Matrix4(
        1.0f, 0.0f, 0.0f, x,
        0.0f, 1.0f, 0.0f, y,
        0.0f, 0.0f, 1.0f, z,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}

Matrix4 translateX(float scalar) {
    return Matrix4(
        1.0f, 0.0f, 0.0f, scalar,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}

Matrix4 translateY(float scalar) {
    return Matrix4(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, scalar,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}

Matrix4 translateZ(float scalar) {
    return Matrix4(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, scalar,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}

Matrix4 rotate(float angle, const Vector& axis) {
    angle = radians(angle);

    float cosine = cosf(angle);
    float sine = sinf(angle);

    Vector nAxis = axis;
    if(nAxis != Vector(0.0f, 0.0f, 0.0f)) {
        nAxis = normalize(axis);
    }

    Vector temp = (1.0f - cosine) * nAxis;

    return Matrix4(
        cosine + temp.x * nAxis.x,
        temp.x * nAxis.y + sine * nAxis.z,
        temp.x * nAxis.z - sine * nAxis.y,

        temp.y * nAxis.x - sine * nAxis.z,
        cosine + temp.y * nAxis.y,
        temp.y * nAxis.z + sine * nAxis.x,

        temp.x * nAxis.x + sine * nAxis.y,
        temp.x * nAxis.y - sine * nAxis.x,
        cosine + temp.x * nAxis.z
    );
}

Matrix4 rotateX(float angle) {
    angle = radians(angle);

    const float cosine = cosf(angle);
    const float sine = sinf(angle);

    return Matrix4(
        1.0f, 0.0f, 0.0f,
        0.0f, cosine, -sine,
        0.0f, sine, cosine
    );
}

Matrix4 rotateY(float angle) {
    angle = radians(angle);

    const float cosine = cosf(angle);
    const float sine = sinf(angle);

    return Matrix4(
        cosine, 0.0f, sine,
        0.0f, 1.0f, 0.0f,
        -sine, 0.0f, cosine
    );
}

Matrix4 rotateZ(float angle) {
    angle = radians(angle);

    const float cosine = cosf(angle);
    const float sine = sinf(angle);

    return Matrix4(
        cosine, -sine, 0.0f,
        sine, cosine, 0.0f,
        0.0f, 0.0f, 1.0f
    );
}

Matrix4 lookAt(const Point& eye, const Point& center, const Vector& up) {
    const Vector FRONT = normalize(center - eye);
    const Vector SIDE = normalize(cross(FRONT, up));
    const Vector UP = normalize(cross(SIDE, FRONT));

    return Matrix4(
        SIDE.x, SIDE.y, SIDE.z, -dot(SIDE, eye),
        UP.x, UP.y, UP.z, -dot(UP, eye),
        -FRONT.x, -FRONT.y, -FRONT.z, dot(FRONT, eye),
        0.0f, 0.0f, 0.0f, 1.0f
    );
}

Matrix4 perspective(float fov, float aspect, float near, float far) {
    return Matrix4(
        1.0f / (aspect * tanf(0.5f * fov)), 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f / tanf(0.5f * fov), 0.0f, 0.0f,
        0.0f, 0.0f, -(far + near) / (far - near), -(2.0f * far * near) / (far - near),
        0.0f, 0.0f, -1.0f, 0.0f
    );
}