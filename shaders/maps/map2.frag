#version 460 core

uniform float time;

float SDF_Cylinder(in vec3 pos, in vec3 c);
float SDF_Plane(in vec3 pos, in vec3 normal, in float height);
mat2 rotation2D(in float angle);
float smin(in float a, in float b, in float t);
vec4 sDifferenceSDF(vec4 a, vec4 b, in float t);

vec4 map(in vec3 pos) {
    vec3 p = pos;

    vec4 cylinder;
    cylinder.rgb = vec3(0.9f, 0.45f, 0.75f);
    p.xz += vec2(time);
    p.xy *= rotation2D(radians(90.0f));
    p.yz = mod(p.yz, 2.0f) - 0.5f;
    cylinder.w = SDF_Cylinder(p, vec3(0.5f));

    p = pos;
    p.zy *= rotation2D(radians(90.0f));
    p.xy = mod(p.xy, 2.0f) - 0.5f;
    cylinder.w = smin(cylinder.w, SDF_Cylinder(p, vec3(0.5f)), 0.2f);

    vec4 ground;
    ground.rgb = vec3(0.75f, 0.3f, 0.6f);
    ground.w = SDF_Plane(pos, vec3(0.0f, 1.0f, 0.0f), 0.5f + 0.5f * sin(time));

    return sDifferenceSDF(ground, cylinder, 0.5f);
}
