/***************************************************************************************************
 * @file  transformations.hpp
 * @brief Declaration of functions for 3D transformations
 **************************************************************************************************/

#include "Matrix4.hpp"
#include "vec4.hpp"
#include "vec3.hpp"

/**
 * @brief Calculates the scaling matrix that scales by the same factor in all 3 directions.
 * @param scalar The scaling factor.
 * @return The scaling matrix.
 */
Matrix4 scale(float scalar);

/**
 * @brief Calculates the scaling matrix that scales by a specific factor in each direction.
 * @param x The scaling factor in the x direction.
 * @param y The scaling factor in the y direction.
 * @param z The scaling factor in the z direction.
 * @return The scaling matrix.
 */
Matrix4 scale(float x, float y, float z);

/**
 * @brief Calculates the scaling matrix that only scales in the x direction.
 * @param scalar The scaling factor.
 * @return The scaling matrix.
 */
Matrix4 scaleX(float scalar);

/**
 * @brief Calculates the scaling matrix that only scales in the y direction.
 * @param scalar The scaling factor.
 * @return The scaling matrix.
 */
Matrix4 scaleY(float scalar);

/**
 * @brief Calculates the scaling matrix that only scales in the z direction.
 * @param scalar The scaling factor.
 * @return The scaling matrix.
 */
Matrix4 scaleZ(float scalar);

/**
 * @brief Calculates the translation matrix that displaces by a specific vector.
 * @param vector The translation vector.
 * @return The translation matrix.
 */
Matrix4 translate(const Vector& vector);

/**
 * @brief Calculates the translation matrix that displaces by a specific amount in each direction.
 * @param x The displacement in the x direction.
 * @param y The displacement in the y direction.
 * @param z The displacement in the z direction.
 * @return The translation matrix.
 */
Matrix4 translate(float x, float y, float z);

/**
 * @brief Calculates the translation matrix that only displaces in the x direction.
 * @param scalar The displacement amount.
 * @return The translation matrix.
 */
Matrix4 translateX(float scalar);

/**
 * @brief Calculates the translation matrix that only displaces in the y direction.
 * @param scalar The displacement amount.
 * @return The translation matrix.
 */
Matrix4 translateY(float scalar);

/**
 * @brief Calculates the translation matrix that only displaces in the z direction.
 * @param scalar The displacement amount.
 * @return The translation matrix.
 */
Matrix4 translateZ(float scalar);

/**
 * @brief Calculates the rotation matrix that rotates around an axis by a certain angle.
 * @param angle The rotation angle in degrees.
 * @param axis The rotation axis.
 * @return The rotation matrix.
 */
Matrix4 rotate(float angle, const Vector& axis);

/**
 * @brief Calculates the rotation matrix that rotates around the X axis by a certain angle.
 * @param angle The rotation angle in degrees.
 * @return The rotation matrix.
 */
Matrix4 rotateX(float angle);

/**
 * @brief Calculates the rotation matrix that rotates around the Y axis by a certain angle.
 * @param angle The rotation angle in degrees.
 * @return The rotation matrix.
 */
Matrix4 rotateY(float angle);

/**
 * @brief Calculates the rotation matrix that rotates around the Z axis by a certain angle.
 * @param angle The rotation angle in degrees.
 * @return The rotation matrix.
 */
Matrix4 rotateZ(float angle);

/**
 * @brief Calculates the 'look at' matrix. That allows to simulate a camera.
 * @param eye The position of the camera.
 * @param center The target of the camera.
 * @param up The up vector of the camera.
 * @return The 'look at' matrix.
 */
Matrix4 lookAt(const Point& eye, const Point& center, const Vector& up);

/**
 * @brief Calculates the perspective matrix.
 * @param fov The field of vue value. Sets how large the viewspace is. For a realistic view it is
 * usually set to 45 degrees.
 * @param aspect The aspect ration. Is calculated by dividing the window's width by its height.
 * @param near, far The near and far plane. Usually set to respectively 0.1 and 100, all vertices
 * between the two planes are renderered, those outside won't.
 * @return The perspective matrix.
 */
Matrix4 perspective(float fov, float aspect, float near, float far);