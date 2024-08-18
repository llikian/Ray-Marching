/***************************************************************************************************
 * @file  signed_distance_functions.glsl
 * @brief Implementation of signed distance functions
 **************************************************************************************************/

float SDF_Plane(in vec3 pos, in vec3 normal, in float height) {
    return dot(pos, normalize(normal)) + height;
}

float SDF_Box(in vec3 pos, in vec3 dimensions) {
    vec3 q = abs(pos) - dimensions;
    return length(max(q, 0.0f)) + min(max(q.x, max(q.y, q.z)), 0.0f);
}

float SDF_RoundBox(in vec3 pos, in vec3 dimensions, in float radius) {
    vec3 q = abs(pos) - dimensions + radius;
    return length(max(q, 0.0f)) + min(max(q.x, max(q.y, q.z)), 0.0f) - radius;
}

float SDF_Sphere(in vec3 pos, in float radius) {
    return length(pos) - radius;
}

float SDF_Cylinder(in vec3 pos, in vec3 c) {
    return length(pos.xz - c.xy) - c.z;
}

float SDF_CappedCylinder(in vec3 pos, in float height, in float radius) {
    vec2 d = abs(vec2(length(pos.xz), pos.y)) - vec2(radius, height);
    return min(max(d.x, d.y), 0.0f) + length(max(d, 0.0f));
}

float SDF_Cone(in vec3 pos, in vec2 c, in float height) {
    // c is the sin/cos of the angle, h is height
    vec2 q = height * vec2(c.x / c.y, -1.0f);

    vec2 w = vec2(length(pos.xz), pos.y);
    vec2 a = w - q * clamp(dot(w,q) / dot(q, q), 0.0f, 1.0f);
    vec2 b = w - q * vec2(clamp(w.x / q.x, 0.0f, 1.0f), 1.0f);
    float k = sign(q.y);
    float d = min(dot(a, a), dot(b, b));
    float s = max(k * (w.x * q.y - w.y * q.x), k * (w.y - q.y));
    return sqrt(d) * sign(s);
}

float SDF_Torus(in vec3 pos, in vec2 t) {
    vec2 q = vec2(length(pos.xz) - t.x, pos.y);
    return length(q) - t.y;
}