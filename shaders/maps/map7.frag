#version 460 core

float SDF_Box(in vec3 pos, in vec3 dimensions);
float SDF_Cone(in vec3 pos, in vec2 c, in float height);
float SDF_Sphere(in vec3 pos, in float radius);
mat2 rotation2D(in float angle);
float smin(in float a, in float b, in float t);
vec4 unionSDF(vec4 a, vec4 b);
vec4 sUnionSDF(vec4 a, vec4 b, in float t);

vec4 map(in vec3 pos) {
    vec3 p = pos;

    vec4 ground = vec4(0.545f, 0.851f, 0.42f, p.y);

    p = pos;
    p.y -= 5.0f;
    vec4 house;
    house.rgb = vec3(0.6f, 0.565f, 0.506f);
    house.w = SDF_Box(p, vec3(5.0f));

    p = pos;
    p.y -= 4.05f;
    house.w = max(house.w, -SDF_Box(p, vec3(4.0f)));

    p = pos - vec3(0.0f, 3.05f, 5.0f);
    house.w = max(house.w, -SDF_Box(p, vec3(2.0f, 3.0f, 2.0f)));

    p = pos;
    p.y -= 1.5f;
    vec4 snowman;
    snowman.rgb = vec3(1.0f);
    snowman.w = SDF_Sphere(p, 1.5f);

    p.y -= 2.25f;
    snowman.w = smin(snowman.w, SDF_Sphere(p, 1.0f), 0.15f);

    p.y -= 1.5f;
    snowman.w = smin(snowman.w, SDF_Sphere(p, 0.6f), 0.15f);

    vec4 eyes;
    eyes.rgb = vec3(0.0f);
    eyes.w = SDF_Sphere(pos - vec3(0.3f, 5.4f, 0.5f), 0.05f);
    eyes.w = min(eyes.w, SDF_Sphere(pos - vec3(-0.3f, 5.4f, 0.5f), 0.05f));

    p = pos - vec3(0.0f, 5.25f, 1.1f);
    p.zy *= rotation2D(radians(90.0f));
    vec4 nose;
    nose.rgb = vec3(0.871f, 0.584f, 0.184f);
    nose.w = SDF_Cone(p, vec2(sin(radians(7.5)), cos(radians(7.5))), 0.5f);

    vec4 result = ground;
    result = unionSDF(result, house);
    result = unionSDF(result, sUnionSDF(snowman, unionSDF(eyes, nose), 0.01f));

    return result;
}
