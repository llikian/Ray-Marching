/***************************************************************************************************
 * @file  render.frag
 * @brief Implementation of functions regarding rendering
 **************************************************************************************************/

#include "raymarching.glsl"
#include "lighting.glsl"

vec2 getUV(in vec2 offset) {
    return (2.0f * (gl_FragCoord.xy + offset) - resolution) / resolution.y;
}

vec3 render(in vec2 uvOffset) {
    const vec3 background = vec3(0.125f, 0.5f, 0.8f);
    vec3 color = background;

    vec2 uv = getUV(uvOffset);
    Ray ray = Ray(cameraPos, normalize(cameraFront + uv.x * cameraRight + uv.y * cameraUp));
    float distance = raymarch(ray, color);

    if(distance < MAX_DISTANCE) {
        if(hasLighting) {
            color *= phongLighting(ray, ray.origin + ray.direction * distance);
            color = mix(background, color, exp(-0.00002f * distance * distance)); // fog
        } else {
            color *= vec3(0.15f * distance);
        }
    } else {
        color = background + max(0.75f * ray.direction.y, 0.0f);
    }

    return color;
}

vec3 renderAntiAliasing4() {
    vec4 e = vec4(0.125f, -0.125f, 0.375f, -0.375f);
    return 0.25f * (render(e.xz) + render(e.yw) + render(e.wx) + render(e.zy));
}
