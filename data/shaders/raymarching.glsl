/***************************************************************************************************
 * @file  raymarching.glsl
 * @brief Implementation of functions regarding raymarching
 **************************************************************************************************/

#include "maps.glsl"

#define MAX_STEPS 256
#define MIN_DISTANCE 0.0001f
#define MAX_DISTANCE 1000.0f

struct Ray {
    vec3 origin;
    vec3 direction;
};

vec4 map(in vec3 pos) {
    switch(active_scene) {
        case 0: return map1(pos);
        case 1: return map2(pos);
        case 2: return map3(pos);
        case 3: return map4(pos);
        case 4: return map5(pos);
        case 5: return map6(pos);
        case 6: return map7(pos);
        case 7: return map8(pos);
        case 8: return map9(pos);
        default: return map1(pos);
    }
}

float raymarch(in Ray ray, inout vec3 color) {
    vec4 distance;
    float distanceFromOrigin = MIN_DISTANCE;
    int steps = 0;

    for(uint i = 0 ; i < MAX_STEPS ; ++i) {
        if(distanceFromOrigin > MAX_DISTANCE) {
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
    Ray ray;

    ray.origin = rayOrigin;
    ray.direction = rayDirection;

    vec3 color = vec3(0.0f);

    return raymarch(ray, color);
}