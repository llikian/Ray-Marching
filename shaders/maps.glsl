/***************************************************************************************************
 * @file  maps.glsl
 * @brief Implementation of different raymarching scenes
 **************************************************************************************************/

#include "signed_distance_functions.glsl"
#include "transformations.glsl"
#include "utility.glsl"

const vec3 RED = vec3(1.0f, 0.0f, 0.0f);
const vec3 GREEN = vec3(0.0f, 1.0f, 0.0f);
const vec3 BLUE = vec3(0.0f, 0.0f, 1.0f);

vec4 map1(in vec3 pos) {
    hasShadows = false;

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

vec4 map2(in vec3 pos) {
    hasShadows = false;

    vec3 p;

    float repetition = 12.0f;
    float radius = 2.0f;
    vec3 translation = 5.5f * vec3(radius, 0.0f, radius);

    vec4 cylinder;
    cylinder.rgb = vec3(0.25f, 0.8f, 0.4f);
    p = pos + translation;
    p = mod(p, repetition) - 0.25f * repetition;
    cylinder.w = SDF_Cylinder(p, vec3(radius));

    p = mod(pos.zxy + translation, repetition) - 0.25f * repetition;
    cylinder.w = smin(cylinder.w, SDF_Cylinder(p, vec3(radius)), 0.1f);

    p = mod(pos.yzx + translation, repetition) - 0.25f * repetition;
    cylinder.w = smin(cylinder.w, SDF_Cylinder(p, vec3(radius)), 0.1f);

    vec4 cube;
    cube.rgb = vec3(0.25f, 0.5f, 0.4f);
    p = mod(pos, repetition) - 0.5f * repetition;
    cube.w = -SDF_RoundBox(p, vec3(4.0f), 1.0f);

    return sDifferenceSDF(cube, cylinder, 0.5f);
}

vec4 map3(in vec3 pos) {
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

vec4 map4(in vec3 pos) {
    hasShadows = false;

    vec3 p;

    vec4 box = vec4(RED, 0.0f);
    vec4 beams = vec4(BLUE, 0.0f);

    float distance = 8.0f;
    float smoothing = 1.0f;

    vec4 result;

    p = pos - vec3(distance, -2.0f * -distance, 0.0f);
    box.w = SDF_Box(p, vec3(2.0f));
    result = box;

    p = pos - vec3(-distance, -2.0f * -distance, 0.0f);
    beams.w = SDF_Box(p, vec3(1.0f, 3.0f, 1.0f));
    beams.w = min(beams.w, SDF_Box(p, vec3(3.0f, 1.0f, 1.0f)));
    beams.w = min(beams.w, SDF_Box(p, vec3(1.0f, 1.0f, 3.0f)));
    result = unionSDF(result, beams);

    p = pos - vec3(-distance, 0.0f, 0.0f);
    box.w = SDF_Box(p, vec3(2.0f));
    beams.w = SDF_Box(p, vec3(1.0f, 3.0f, 1.0f));
    beams.w = min(beams.w, SDF_Box(p, vec3(3.0f, 1.0f, 1.0f)));
    beams.w = min(beams.w, SDF_Box(p, vec3(1.0f, 1.0f, 3.0f)));
    result = unionSDF(result, unionSDF(box, beams));

    p = pos;
    box.w = SDF_Box(p, vec3(2.0f));
    beams.w = SDF_Box(p, vec3(1.0f, 3.0f, 1.0f));
    beams.w = min(beams.w, SDF_Box(p, vec3(3.0f, 1.0f, 1.0f)));
    beams.w = min(beams.w, SDF_Box(p, vec3(1.0f, 1.0f, 3.0f)));
    result = unionSDF(result, intersectSDF(box, beams));

    p = pos - vec3(distance, 0.0f, 0.0f);
    box.w = SDF_Box(p, vec3(2.0f));
    beams.w = SDF_Box(p, vec3(1.0f, 3.0f, 1.0f));
    beams.w = min(beams.w, SDF_Box(p, vec3(3.0f, 1.0f, 1.0f)));
    beams.w = min(beams.w, SDF_Box(p, vec3(1.0f, 1.0f, 3.0f)));
    result = unionSDF(result, differenceSDF(box, beams));

    p = pos - vec3(-distance, distance, 0.0f);
    box.w = SDF_Box(p, vec3(2.0f));
    beams.w = SDF_Box(p, vec3(1.0f, 3.0f, 1.0f));
    beams.w = min(beams.w, SDF_Box(p, vec3(3.0f, 1.0f, 1.0f)));
    beams.w = min(beams.w, SDF_Box(p, vec3(1.0f, 1.0f, 3.0f)));
    result = unionSDF(result, sUnionSDF(box, beams, smoothing));

    p = pos - vec3(0.0f, distance, 0.0f);
    box.w = SDF_Box(p, vec3(2.0f));
    beams.w = SDF_Box(p, vec3(1.0f, 3.0f, 1.0f));
    beams.w = min(beams.w, SDF_Box(p, vec3(3.0f, 1.0f, 1.0f)));
    beams.w = min(beams.w, SDF_Box(p, vec3(1.0f, 1.0f, 3.0f)));
    result = unionSDF(result, sIntersectSDF(box, beams, smoothing));

    p = pos - vec3(distance, distance, 0.0f);
    box.w = SDF_Box(p, vec3(2.0f));
    beams.w = SDF_Box(p, vec3(1.0f, 3.0f, 1.0f));
    beams.w = min(beams.w, SDF_Box(p, vec3(3.0f, 1.0f, 1.0f)));
    beams.w = min(beams.w, SDF_Box(p, vec3(1.0f, 1.0f, 3.0f)));
    result = unionSDF(result, sDifferenceSDF(box, beams, smoothing));

    return result;
}

vec4 map5(in vec3 pos) {
    vec3 p = pos;
    vec4 ground;
    ground.rgb = vec3(1.0f);
    ground.w= SDF_Plane(p, vec3(0.0f, 1.0f, 0.0f), 1.0f);

    return ground;
}

vec4 map6(in vec3 pos) {
    hasShadows = false;

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

vec4 map7(in vec3 pos) {
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

vec4 map8(in vec3 pos) {
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

vec4 map9(in vec3 pos) {
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

vec4 map10(in vec3 pos) {
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

vec4 map11(in vec3 pos) {
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

vec4 map12(in vec3 pos) {
    vec3 p = pos;

    p.y += 1.0f;
    float check = floor(pos.x / 2.0f) + floor(pos.y / 2.0f) + floor(pos.z / 2.0f);
    vec4 box;
    box.rgb = mod(check, 2.0f) == 0.0f ? vec3(0.89f, 0.816f, 0.702f) : vec3(0.188f, 0.173f, 0.145f);
    box.w = SDF_Box(p, vec3(8.0f, 1.0f, 8.0f));

    vec4 result = box;

    return result;
}