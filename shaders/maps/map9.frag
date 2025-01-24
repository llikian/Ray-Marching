#version 460 core

const vec3 GREEN = vec3(0.0f, 1.0f, 0.0f);
const vec3 BLUE = vec3(0.0f, 0.0f, 1.0f);

uniform float time;

float SDF_Sphere(in vec3 pos, in float radius);
vec3 checker(in vec3 pos, in vec3 col1, in vec3 col2);
vec4 unionSDF(vec4 a, vec4 b);

vec4 map(in vec3 pos) {
    vec3 p = pos;

    vec4 ground;
    ground.rgb = checker(p, vec3(0.89f, 0.847f, 0.471f), vec3(0.89f, 0.337f, 0.306f));
    ground.w = p.y + 10.0f;

    vec3 factor = time * vec3(2.0f, 0.5f, 3.0f);
    float displacement = sin(p.x + factor.x) * sin(p.y + factor.y) * sin(p.z + factor.z);
    vec4 sphere;
    sphere.rgb = mix(BLUE, GREEN, 0.5f + 0.5f * displacement);
    sphere.w = SDF_Sphere(p, 10.0f + displacement);

    vec4 result = unionSDF(ground, sphere);

    return result;
}
