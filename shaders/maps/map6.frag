#version 460 core

const vec3 RED = vec3(1.0f, 0.0f, 0.0f);
const vec3 GREEN = vec3(0.0f, 1.0f, 0.0f);

uniform float time;

float SDF_Box(in vec3 pos, in vec3 dimensions);
float SDF_Plane(in vec3 pos, in vec3 normal, in float height);
float SDF_Sphere(in vec3 pos, in float radius);
mat2 rotation2D(in float angle);
vec4 sUnionSDF(vec4 a, vec4 b, in float t);
vec4 differenceSDF(vec4 a, vec4 b);
vec4 sDifferenceSDF(vec4 a, vec4 b, in float t);

vec4 map(in vec3 pos) {
    vec3 p = pos;
    vec4 ground;
    ground.rgb = RED;
    ground.w = SDF_Plane(p, GREEN, 0.5f + 0.5f * sin(time));

    p = pos;
    p.y += 0.5f;
    p.xy *= rotation2D(radians(90.0f));
    p.yz = mod(p.yz, 2.0f) - 1.0f;
    vec4 cube;
    cube.rgb = vec3(0.851f, 0.584f, 0.251f);
    cube.w = SDF_Box(p, vec3(0.5f));

    p = pos;
    p.y -= time;
    p.xz = mod(p.xz, 2.0f) - 1.0f;
    p.y = mod(p.y, 4.0f) - 2.0f;
    vec4 sphere;
    sphere.rgb = vec3(0.851f, 0.251f, 0.682f);
    sphere.w = SDF_Sphere(p, 0.25f);

    p = pos;
    vec4 ceiling;
    ceiling.rgb = vec3(0.0f);
    ceiling.w = SDF_Plane(p, vec3(0.0f, -1.0f, 0.0f), 4.0f);

    vec4 result;
    result = sUnionSDF(sDifferenceSDF(ground, cube, 0.25f), differenceSDF(sphere, ceiling), 0.25f);

    return result;
}
