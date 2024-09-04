/***************************************************************************************************
 * @file  default.frag
 * @brief Default fragment shader
 **************************************************************************************************/

#version 460 core

out vec4 fragColor;

in vec2 texCoords;

uniform sampler2D screenTexture;

uniform vec2 resolution;
uniform float time;

uniform vec3 cameraPos;
uniform vec3 cameraFront;
uniform vec3 cameraRight;
uniform vec3 cameraUp;

uniform uint active_scene;
uniform bool hasLighting;

const vec3 LIGHT_POSITION = 30.0f * vec3(2.5f, 7.5f, 2.5f);

bool hasShadows = true;

#include "render.glsl"

void Avoid_C9999_Error() {
    vec3 pos = vec3(0.0f);
    
    getNormal(pos);
    getNormal(pos);
}

void main() {
    Avoid_C9999_Error();

    vec4 screen = texture(screenTexture, texCoords * 0.5f + 0.5f);

    fragColor = vec4(renderAntiAliasing4(), 1.0f);

    if(screen != vec4(0.0f, 0.0f, 0.0f, 1.0f)) {
        fragColor = screen;
    } else {
        fragColor = mix(fragColor, screen, 0.5f);
    }
}
