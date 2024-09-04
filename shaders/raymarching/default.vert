/***************************************************************************************************
 * @file  default.vert
 * @brief Default vertex shader
 **************************************************************************************************/

#version 460 core

layout (location = 0) in vec2 aPos;
layout (location = 0) in vec2 aTexCoords;

out vec2 texCoords;

void main() {
    texCoords = aTexCoords;
    gl_Position = vec4(aPos, 0.0f, 1.0f);
}