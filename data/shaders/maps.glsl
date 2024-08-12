/***************************************************************************************************
 * @file  maps.glsl
 * @brief Implementation of different raymarching scenes
 **************************************************************************************************/

#include "signed_distance_functions.glsl"
#include "transformations.glsl"
#include "utility.glsl"

vec4 map1(in vec3 pos) {
    vec3 p;
    
    p = pos;
    p.x -= 3.0f * sin(time);
    float sphere = SDF_Sphere(p, 2.0f);

    p = pos;
    p.y -= time * 0.5f;
    p = fract(p) - 0.5f;
    p.xz *= rotation2D(time);
    p.zy *= rotation2D(time);
    float box = SDF_Box(p, vec3(0.1f));

    float ground = pos.y;

    vec3 color = vec3(0.5f, 0.0f, 0.5f);
    return vec4(color, min(ground, smin(sphere, box, 2.0f)));
}

vec4 map2(in vec3 pos) {
    vec3 p;
    pos -= vec3(6.0f, -3.0f, -1.0f);

    p = pos;
    p = mod(p, 2.0f) - 0.75f;
    float cylinder = SDF_Cylinder(p, vec3(0.25f));

    p = pos;
    p.xy *= rotation2D(radians(90.0f));
    p = mod(p, 2.0f) - 0.75f;
    cylinder = smin(cylinder, SDF_Cylinder(p, vec3(0.25f)), 0.1f);

    p = pos;
    p.yz *= rotation2D(radians(90.0f));
    p = mod(p, 2.0f) - 0.75f;
    cylinder = smin(cylinder, SDF_Cylinder(p, vec3(0.25f)), 0.1f);

    p = pos;
    p = mod(p, 12.0f) - 6.0f;
    float cube = SDF_RoundBox(p, vec3(4.0f), 1.0f);

    vec3 color = vec3(0.5f, 0.0f, 0.5f);
    return vec4(color, smax(-cube, -cylinder, 0.5f));
    //    return smax(cube, cylinder, 0.5f);
    //    return smax(cube, -cylinder, 0.5f);
}

vec4 map3(in vec3 pos) {
    vec3 p = pos;
    p.xz += vec2(time);
    p.xy *= rotation2D(radians(90.0f));
    p.yz = mod(p.yz, 2.0f) - 0.5f;
    float cylinder = SDF_Cylinder(p, vec3(0.5f));

    p = pos;
    p.zy *= rotation2D(radians(90.0f));
    p.xy = mod(p.xy, 2.0f) - 0.5f;
    cylinder = smin(cylinder, SDF_Cylinder(p, vec3(0.5f)), 0.2f);
    
    float ground = SDF_Plane(pos, vec3(0.0f, 1.0f, 0.0f), 0.5f + 0.5f * sin(time));

    vec3 color = vec3(0.5f, 0.0f, 0.5f);
    return vec4(color, smax(ground, -cylinder, 0.5f));
}

vec4 map4(in vec3 pos) {
    vec3 p;
    float result;

    float box, beams;

    float distance = 8.0f;
    float smoothing = 1.0f;

    p = pos - vec3(distance, -2.0f * -distance, 0.0f);
    box = SDF_Box(p, vec3(2.0f));
    result = box;

    p = pos - vec3(-distance, -2.0f * -distance, 0.0f);
    beams = SDF_Box(p, vec3(1.0f, 3.0f, 1.0f));
    beams = min(beams, SDF_Box(p, vec3(3.0f, 1.0f, 1.0f)));
    beams = min(beams, SDF_Box(p, vec3(1.0f, 1.0f, 3.0f)));
    result = min(result, beams);

    p = pos - vec3(distance, 0.0f, 0.0f);
    box = SDF_Box(p, vec3(2.0f));
    beams = SDF_Box(p, vec3(1.0f, 3.0f, 1.0f));
    beams = min(beams, SDF_Box(p, vec3(3.0f, 1.0f, 1.0f)));
    beams = min(beams, SDF_Box(p, vec3(1.0f, 1.0f, 3.0f)));
    result = min(result, min(box, beams));

    p = pos;
    box = SDF_Box(p, vec3(2.0f));
    beams = SDF_Box(p, vec3(1.0f, 3.0f, 1.0f));
    beams = min(beams, SDF_Box(p, vec3(3.0f, 1.0f, 1.0f)));
    beams = min(beams, SDF_Box(p, vec3(1.0f, 1.0f, 3.0f)));
    result = min(result, max(box, beams));

    p = pos - vec3(-distance, 0.0f, 0.0f);
    box = SDF_Box(p, vec3(2.0f));
    beams = SDF_Box(p, vec3(1.0f, 3.0f, 1.0f));
    beams = min(beams, SDF_Box(p, vec3(3.0f, 1.0f, 1.0f)));
    beams = min(beams, SDF_Box(p, vec3(1.0f, 1.0f, 3.0f)));
    result = min(result, max(box, -beams));

    p = pos - vec3(distance, distance, 0.0f);
    box = SDF_Box(p, vec3(2.0f));
    beams = SDF_Box(p, vec3(1.0f, 3.0f, 1.0f));
    beams = min(beams, SDF_Box(p, vec3(3.0f, 1.0f, 1.0f)));
    beams = min(beams, SDF_Box(p, vec3(1.0f, 1.0f, 3.0f)));
    result = min(result, smin(box, beams, smoothing));

    p = pos - vec3(0.0f, distance, 0.0f);
    box = SDF_Box(p, vec3(2.0f));
    beams = SDF_Box(p, vec3(1.0f, 3.0f, 1.0f));
    beams = min(beams, SDF_Box(p, vec3(3.0f, 1.0f, 1.0f)));
    beams = min(beams, SDF_Box(p, vec3(1.0f, 1.0f, 3.0f)));
    result = min(result, smax(box, beams, smoothing));

    p = pos - vec3(-distance, distance, 0.0f);
    box = SDF_Box(p, vec3(2.0f));
    beams = SDF_Box(p, vec3(1.0f, 3.0f, 1.0f));
    beams = min(beams, SDF_Box(p, vec3(3.0f, 1.0f, 1.0f)));
    beams = min(beams, SDF_Box(p, vec3(1.0f, 1.0f, 3.0f)));
    result = min(result, smax(box, -beams, smoothing));

    vec3 color = vec3(0.5f, 0.0f, 0.5f);
    return vec4(color, result);
}

vec4 map5(in vec3 pos) {
    vec3 p = pos;
    float ground = SDF_Plane(p, vec3(0.0f, 1.0f, 0.0f), 1.0f);
    float sphere = SDF_Sphere(p, 1.0f);
    float cube = SDF_Box(p, vec3(1.0f));

    p = pos;
    p.z -= sin(time);
    p.y *= 4.0f;
    // Need to divide by biggest scaling factor to keep lighting consistent
    float disk = SDF_Sphere(p, 2.0f) / 4.0f;

    vec3 color = vec3(0.5f, 0.0f, 0.5f);
    return vec4(color, min(ground, smax(cube, -disk, 0.25f)));
    //    return min(ground, mix(cube, sphere, 0.5f + 0.5f * sin(time)));
    //    return min(ground, max(abs(sphere) - 0.05f, -SDF_Plane(pos, vec3(1.0f, -1.0f, 0.0f), 0.0f)));
}

vec4 map6(in vec3 pos) {
    float ground = SDF_Plane(pos, vec3(0.0f, 1.0f, 0.0f), 1.0f);

    pos = clamp(pos, -8.0f, 8.0f);
    pos = mod(pos, 4.0f) - 2.0f;

    float cube = SDF_Box(pos, vec3(1.0f));

    vec3 dim = vec3(0.5f, 1.5f, 0.5f);
    float cross = min3(SDF_Box(pos, dim.xyz), SDF_Box(pos, dim.yzx), SDF_Box(pos, dim.zxy));

    vec3 color = vec3(0.5f, 0.0f, 0.5f);
    return vec4(color, max(cube, -cross));
}

vec4 map7(in vec3 pos) {
    vec3 p = pos;
    vec4 ground;
    ground.rgb = vec3(1.0f, 0.0f, 0.0f);
    ground.w = SDF_Plane(p, vec3(0.0f, 1.0f, 0.0f), 0.5f + 0.5f * sin(time));

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
