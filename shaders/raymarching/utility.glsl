/***************************************************************************************************
 * @file  utility.glsl
 * @brief Implementation of various utility functions
 **************************************************************************************************/

float smin(in float a, in float b, in float t) {
    float h = clamp(0.5f + 0.5f * (a - b) / t, 0.0f, 1.0f);
    return mix(a, b, h) - t * h * (1.0f - h);
}

float smax(in float a, in float b, in float t) {
    float h = clamp(0.5f - 0.5f * (a - b) / t, 0.0f, 1.0f);
    return mix(a, b, h) + t * h * (1.0f - h);
}

float min3(in float a, in float b, in float c) {
    return min(a, min(b, c));
}

float max3(in float a, in float b, in float c) {
    return max(a, max(b, c));
}

vec4 intersectSDF(vec4 a, vec4 b) {
    return a.w > b.w ? a : b;
}

vec4 unionSDF(vec4 a, vec4 b) {
    return a.w < b.w ? a : b;
}

vec4 differenceSDF(vec4 a, vec4 b) {
    return a.w > -b.w ? a : vec4(b.rgb, -b.w);
}

vec4 sIntersectSDF(vec4 a, vec4 b, in float t) {
    float h = clamp(0.5f - 0.5f * (a.w - b.w) / t, 0.0f, 1.0f);
    return vec4(mix(a.rgb, b.rgb, 1.0f - h), mix(a.w, b.w, h) + t * h * (1.0f - h));
}

vec4 sUnionSDF(vec4 a, vec4 b, in float t) {
    float h = clamp(0.5f + 0.5f * (a.w - b.w) / t, 0.0f, 1.0f);
    return vec4(mix(a.rgb, b.rgb, h), mix(a.w, b.w, h) - t * h * (1.0f - h));
}

vec4 sDifferenceSDF(vec4 a, vec4 b, in float t) {
    float h = clamp(0.5f - 0.5f * (a.w + b.w) / t, 0.0f, 1.0f);
    return vec4(mix(a.rgb, b.rgb, h), mix(a.w, -b.w, h) + t * h * (1.0f - h));
}

vec3 checker(in vec3 pos, in vec3 col1, in vec3 col2) {
    return mod(floor(pos.x) + floor(pos.z), 2.0f) == 0.0f ? col1 : col2;
}