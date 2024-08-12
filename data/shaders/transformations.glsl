/***************************************************************************************************
 * @file  transformations.frag
 * @brief Implementation of 3D transformation functions
 **************************************************************************************************/

mat2 rotation2D(in float angle) {
    float cosine = cos(angle);
    float sine = sin(angle);

    return mat2(cosine, -sine, sine, cosine);
}

vec3 rotation3D(in vec3 point, in vec3 axis, in float angle) {
    // Rodrigues' rotation formula
    return mix(dot(axis, point) * axis, point, cos(angle)) + cross(axis, point) * sin(angle);
}