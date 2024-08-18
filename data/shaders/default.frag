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

//const vec3 LIGHT_POSITION = 10.0f * vec3(cos(time), 1.0f, sin(time));
const vec3 LIGHT_POSITION = 10.0f * vec3(2.5f, 7.5f, 2.5f);

#include "raymarching.glsl"
#include "lighting.glsl"

void render(inout vec3 color, in vec2 uv) {
    Ray ray = Ray(cameraPos, normalize(cameraFront + uv.x * cameraRight + uv.y * cameraUp));
    float distance = raymarch(ray, color);

    const vec3 background = vec3(0.125f, 0.5f, 0.8f);

    if(distance < MAX_DISTANCE) {
        if(hasLighting) {
            color *= phongLighting(ray, ray.origin + ray.direction * distance);
            color = mix(background, color, exp(-0.00002f * distance * distance)); // fog
        } else {
            color *= vec3(0.15f * distance);
        }
    } else {
        color += background + max(0.75f * ray.direction.y, 0.0f);
    }
}

void main() {
    fragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    render(fragColor.rgb, (gl_FragCoord.xy * 2.0f - resolution) / resolution.y);
}
