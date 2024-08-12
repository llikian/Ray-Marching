/***************************************************************************************************
 * @file  default.frag
 * @brief Default fragment shader
 **************************************************************************************************/

#version 460 core

out vec4 fragColor;

uniform vec2 resolution;
uniform float time;

uniform vec3 cameraPos;
uniform vec3 cameraFront;
uniform vec3 cameraRight;
uniform vec3 cameraUp;

uniform uint active_scene;
uniform bool hasLighting;

//#define LIGHT_POSITION (10.0f * vec3(cos(time), 1.0f, sin(time)))
#define LIGHT_POSITION (vec3(2.5f, 7.5f, 2.5f))

#include "raymarching.glsl"
#include "lighting.glsl"

void main() {
    vec2 uv = (gl_FragCoord.xy * 2.0f - resolution) / resolution.y;

    Ray ray;
    ray.origin = cameraPos;
    ray.direction = normalize(cameraFront + uv.x * cameraRight + uv.y * cameraUp);

    vec3 color = vec3(0.0f);
    float distance = raymarch(ray, color);

    if(hasLighting) {
        color *= phongLighting(ray, ray.origin + ray.direction * distance);
        color *= exp(-0.00001f * distance * distance * distance); // fog
    } else {
        color *= vec3(0.15f * distance);
    }

    fragColor = vec4(color, 1.0f);
}
