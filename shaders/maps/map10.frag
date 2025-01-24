#version 460 core

const vec3 GREEN = vec3(0.0f, 1.0f, 0.0f);
const vec3 BLUE = vec3(0.0f, 0.0f, 1.0f);

uniform float time;

float SDF_Torus(in vec3 pos, in float Radius, in float radius);
vec4 sUnionSDF(vec4 a, vec4 b, in float t);

vec4 map(in vec3 pos) {
    vec3 p = pos;

    float displacement = sin(p.x + 2.0f * time) * sin(p.y + sin(0.25f * time)) * sin(p.z + 3.0f * time);
    float R = 50.0f, r = 2.0f + displacement;

    vec4 torus;
    torus.rgb = mix(BLUE, GREEN, 0.5f + 0.5f * displacement);

    p = pos - vec3(-R / 2.0f, 0.0f, 0.0f);
    torus.w = SDF_Torus(p, R, r);

    p = pos - vec3(R / 2.0f, 0.0f, 0.0f);
    torus = sUnionSDF(torus, vec4(torus.grb, SDF_Torus(p, R, r)), 0.5f);

    p = pos.xzy;
    torus = sUnionSDF(torus, vec4(torus.rgb, SDF_Torus(p, R, r)), 0.5f);

    return torus;
}
