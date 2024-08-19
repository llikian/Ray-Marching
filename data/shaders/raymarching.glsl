/***************************************************************************************************
 * @file  raymarching.glsl
 * @brief Implementation of functions regarding raymarching
 **************************************************************************************************/

#include "maps.glsl"

const uint MAX_STEPS = 128u;
const float MIN_DISTANCE = 0.001f;
const float MAX_DISTANCE = 500.0f;

struct Ray {
    vec3 origin;
    vec3 direction;
};

vec4 map(in vec3 pos) {
    switch(active_scene) {
        case 0u: return map1(pos);
        case 1u: return map2(pos);
        case 2u: return map3(pos);
        case 3u: return map4(pos);
        case 4u: return map5(pos);
        case 5u: return map6(pos);
        case 6u: return map7(pos);
        case 7u: return map8(pos);
        case 8u: return map9(pos);
        case 9u: return map10(pos);
        case 10u: return map11(pos);
        case 11u: return map12(pos);
        default: return map1(pos);
    }
}

float raymarch(in Ray ray, inout vec3 color) {
    vec4 distance;
    float distanceFromOrigin = 0.0f;

    for(uint i = 0u ; i < MAX_STEPS ; ++i) {
        if(distanceFromOrigin >= MAX_DISTANCE) {
            break;
        }

        distance = map(ray.origin + ray.direction * distanceFromOrigin);

        if(abs(distance.w) < MIN_DISTANCE) {
            color = distance.rgb;
            break;
        }

        distanceFromOrigin += distance.w;
    }

    return distanceFromOrigin;
}

float raymarch(in vec3 rayOrigin, in vec3 rayDirection) {
    vec3 color = vec3(0.0f);
    return raymarch(Ray(rayOrigin, rayDirection), color);
}
