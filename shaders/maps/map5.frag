#version 460 core

float SDF_Box(in vec3 pos, in vec3 dimensions);
float SDF_Sphere(in vec3 pos, in float radius);
float min3(in float a, in float b, in float c);
vec4 differenceSDF(vec4 a, vec4 b);

vec4 map(in vec3 pos) {
    float l = 1.0f, s = 4.0f;
    pos = pos - s * clamp(round(pos / s), -l, l);
    vec4 cube;
    cube.rgb = vec3(0.1f, 0.3f, 0.5f);
    cube.w = SDF_Sphere(pos, 1.0f);

    vec3 dim = vec3(0.4f, 1.5f, 0.4f);
    vec4 cross;
    cross.rgb = vec3(0.8f);
    cross.w = min3(SDF_Box(pos, dim.xyz), SDF_Box(pos, dim.yzx), SDF_Box(pos, dim.zxy));

    return differenceSDF(cube, cross);
}
