#version 460 core

float SDF_Cylinder(in vec3 pos, in vec3 c);
float SDF_RoundBox(in vec3 pos, in vec3 dimensions, in float radius);
float smin(in float a, in float b, in float t);
vec4 sDifferenceSDF(vec4 a, vec4 b, in float t);

vec4 map(in vec3 pos) {
    vec3 p;

    float repetition = 12.0f;
    float radius = 2.0f;
    vec3 translation = 5.5f * vec3(radius, 0.0f, radius);

    vec4 cylinder;
    cylinder.rgb = vec3(0.25f, 0.8f, 0.4f);
    p = pos + translation;
    p = mod(p, repetition) - 0.25f * repetition;
    cylinder.w = SDF_Cylinder(p, vec3(radius));

    p = mod(pos.zxy + translation, repetition) - 0.25f * repetition;
    cylinder.w = smin(cylinder.w, SDF_Cylinder(p, vec3(radius)), 0.1f);

    p = mod(pos.yzx + translation, repetition) - 0.25f * repetition;
    cylinder.w = smin(cylinder.w, SDF_Cylinder(p, vec3(radius)), 0.1f);

    vec4 cube;
    cube.rgb = vec3(0.25f, 0.5f, 0.4f);
    p = mod(pos, repetition) - 0.5f * repetition;
    cube.w = -SDF_RoundBox(p, vec3(4.0f), 1.0f);

    return sDifferenceSDF(cube, cylinder, 0.5f);
}
