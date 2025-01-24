#version 460 core

float SDF_Plane(in vec3 pos, in vec3 normal, in float height);

vec4 map(in vec3 pos) {
    vec3 p = pos;
    vec4 ground;
    ground.rgb = vec3(1.0f);
    ground.w = SDF_Plane(p, vec3(0.0f, 1.0f, 0.0f), 1.0f);

    return ground;
}
