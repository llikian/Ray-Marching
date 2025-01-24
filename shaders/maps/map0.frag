#version 460 core

uniform float time;

float SDF_Box(in vec3 pos, in vec3 dimensions);
mat2 rotation2D(in float angle);
vec4 unionSDF(vec4 a, vec4 b);

vec4 map(in vec3 pos) {
    vec3 p = pos;

    vec4 ground = vec4(0.45f, 0.35f, 0.9f, p.y);

    p = pos;
    p.y -= time * 0.5f;
    p = fract(p) - 0.5f;
    p.xz *= rotation2D(time);
    p.zy *= rotation2D(time);
    vec4 box;
    box.rgb = vec3(0.333f, 0.25f, 0.666f);
    box.w = SDF_Box(p, vec3(0.1f));

    vec4 result = unionSDF(ground, box);

    return result;
}
