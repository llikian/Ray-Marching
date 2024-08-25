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

const vec3 LIGHT_POSITION = 30.0f * vec3(2.5f, 7.5f, 2.5f);

#include "render.glsl"

void main() {
    fragColor = vec4(renderAntiAliasing4(), 1.0f);
}
