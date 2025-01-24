#version 460 core

const vec3 RED = vec3(1.0f, 0.0f, 0.0f);
const vec3 GREEN = vec3(0.0f, 1.0f, 0.0f);
const vec3 BLUE = vec3(0.0f, 0.0f, 1.0f);

uniform float time;

float SDF_RoundBox(in vec3 pos, in vec3 dimensions, in float radius);
float SDF_Sphere(in vec3 pos, in float radius);
vec4 unionSDF(vec4 a, vec4 b);
vec4 sUnionSDF(vec4 a, vec4 b, in float t);

vec4 map(in vec3 pos) {
    vec3 p = pos;
    vec4 ground = vec4(1.0f, 1.0f, 1.0f, p.y + 1.0f);

    p = pos;
    vec4 sphere;
    sphere.rgb = vec3(1.0f);
    sphere.w = SDF_Sphere(p, 1.5f);

    p = pos;
    p.y -= 1.0f + sin(time);
    vec4 cube;
    cube.rgb = RED;
    cube.w = SDF_RoundBox(p, vec3(1.0f), 0.1f);

    p = pos;
    p.x -= 1.0f + sin(time);
    vec4 cube2;
    cube2.rgb = GREEN;
    cube2.w = SDF_RoundBox(p, vec3(1.0f), 0.1f);

    p = pos;
    p.x += 1.0f + sin(time);
    vec4 cube3;
    cube3.rgb = BLUE;
    cube3.w = SDF_RoundBox(p, vec3(1.0f), 0.1f);

    cube = sUnionSDF(cube, sUnionSDF(cube2, cube3, 0.5f), 0.5f);

    vec4 result = unionSDF(ground, sUnionSDF(sphere, cube, 0.5f));

    return result;
}
