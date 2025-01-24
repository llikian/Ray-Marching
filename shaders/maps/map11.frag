#version 460 core

float SDF_Box(in vec3 pos, in vec3 dimensions);

vec4 map(in vec3 pos) {
    vec3 p = pos;

    p.y += 1.0f;
    float check = floor(pos.x / 2.0f) + floor(pos.y / 2.0f) + floor(pos.z / 2.0f);
    vec4 box;
    box.rgb = mod(check, 2.0f) == 0.0f ? vec3(0.89f, 0.816f, 0.702f) : vec3(0.188f, 0.173f, 0.145f);
    box.w = SDF_Box(p, vec3(8.0f, 1.0f, 8.0f));

    vec4 result = box;

    return result;
}
