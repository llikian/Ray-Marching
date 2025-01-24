/***************************************************************************************************
 * @file  default.frag
 * @brief Default fragment shader
 **************************************************************************************************/

#version 460 core

out vec4 fragColor;

uniform vec2 resolution;

uniform vec3 cameraPos;
uniform vec3 cameraFront;
uniform vec3 cameraRight;
uniform vec3 cameraUp;

uniform bool hasLighting;
uniform bool hasShadows;

/* ---------------------------------------------------------------------------------------------- *
 *   RAYMARCHING                                                                                  *
 * ---------------------------------------------------------------------------------------------- */

const uint MAX_STEPS = 256u;
const float MIN_DISTANCE = 0.001f;
const float MAX_DISTANCE = 500.0f;

struct Ray {
    vec3 origin;
    vec3 direction;
};

vec4 map(in vec3 pos);

float raymarch(in Ray ray, inout vec3 color) {
    vec4 distance;
    float distanceFromOrigin = 0.0f;

    for(uint i = 0u ; i < MAX_STEPS ; ++i) {
        distance = map(ray.origin + ray.direction * distanceFromOrigin);
        distanceFromOrigin += distance.w;

        if(abs(distance.w) < MIN_DISTANCE || distanceFromOrigin >= MAX_DISTANCE) {
            color = distance.rgb;
            break;
        }
    }

    return distanceFromOrigin;
}

float raymarch(in vec3 rayOrigin, in vec3 rayDirection) {
    vec3 color = vec3(0.0f);
    return raymarch(Ray(rayOrigin, rayDirection), color);
}

/* ---------------------------------------------------------------------------------------------- *
 *   LIGHTING                                                                                     *
 * ---------------------------------------------------------------------------------------------- */

const vec3 LIGHT_POSITION = vec3(2.5f, 7.5f, 2.5f);

vec3 getNormal(in vec3 pos) {
    vec2 epsilon = vec2(MIN_DISTANCE, 0.0f);

    vec3 normal;
    normal.x = map(pos + epsilon.xyy).w - map(pos - epsilon.xyy).w;
    normal.y = map(pos + epsilon.yxy).w - map(pos - epsilon.yxy).w;
    normal.z = map(pos + epsilon.yyx).w - map(pos - epsilon.yyx).w;

    return normalize(normal);
}

float getSoftShadow(vec3 pos) {
    const vec3 lightPos = normalize(LIGHT_POSITION);
    float lightSize = 0.05f;

    float distance;
    float distanceFromOrigin = 0.0f;
    float res = 1.0f;

    for(uint i = 0u; i < MAX_STEPS; ++i) {
        distance = map(pos + lightPos * distanceFromOrigin).w;
        res = min(res, distance / (distanceFromOrigin * lightSize));
        distanceFromOrigin += distance;

        if(abs(distance) < MIN_DISTANCE || distanceFromOrigin > MAX_DISTANCE) {
            break;
        }
    }

    return clamp(res, 0.0f, 1.0f);
}

float getAmbientOcclusion(in vec3 pos, in vec3 normal) {
    float occlusion = 0.0f;
    float weight = 1.0f;

    for(uint i = 0u ; i < 8u ; ++i) {
        float len = 0.01f + 0.02f * float(i * i);
        float distance = map(pos + normal * len).w;
        occlusion += (len - distance) * weight;
    }

    return 1.0f - clamp(0.6f * occlusion, 0.0f, 1.0f);
}

vec3 phongLighting(in Ray ray, in vec3 pos) {
    const vec3 normal = getNormal(pos);

    // Ambient Lighting
    float ambient = 0.2f;

    // Diffuse Lighting
    vec3 lightDirection = normalize(LIGHT_POSITION - pos);
    float diffuse = clamp(dot(normal, lightDirection), 0.0f, 1.0f);

    // Specular lighting
    vec3 reflectionDir = reflect(-lightDirection, normal);
    float specular = 0.25f * pow(max(dot(-ray.direction, reflectionDir), 0.0f), 32.0f);

    // Fresnel Effect
    float fresnel = 1.0f + dot(ray.direction, normal);
    fresnel = 0.25f * fresnel * fresnel * fresnel;

    // Shadows
    float shadows = hasShadows ? getSoftShadow(pos + normal * 0.02f) : 1.0f;

    // Ambient Occlusion
    float occlusion = getAmbientOcclusion(pos, normal);

    return vec3(occlusion * (ambient + fresnel) + shadows * (diffuse + occlusion * specular));
}

/* ---------------------------------------------------------------------------------------------- *
 *   RENDER                                                                                       *
 * ---------------------------------------------------------------------------------------------- */

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

/* ---------------------------------------------------------------------------------------------- *
 *   SIGNED DISTANCE FUNCTIONS                                                                    *
 * ---------------------------------------------------------------------------------------------- */

float SDF_Plane(in vec3 pos, in vec3 normal, in float height) {
    return dot(pos, normalize(normal)) + height;
}

float SDF_Box(in vec3 pos, in vec3 dimensions) {
    vec3 q = abs(pos) - dimensions;
    return length(max(q, 0.0f)) + min(max(q.x, max(q.y, q.z)), 0.0f);
}

float SDF_RoundBox(in vec3 pos, in vec3 dimensions, in float radius) {
    vec3 q = abs(pos) - dimensions + radius;
    return length(max(q, 0.0f)) + min(max(q.x, max(q.y, q.z)), 0.0f) - radius;
}

float SDF_Sphere(in vec3 pos, in float radius) {
    return length(pos) - radius;
}

float SDF_Cylinder(in vec3 pos, in vec3 c) {
    return length(pos.xz - c.xy) - c.z;
}

float SDF_CappedCylinder(in vec3 pos, in float height, in float radius) {
    vec2 d = abs(vec2(length(pos.xz), pos.y)) - vec2(radius, height);
    return min(max(d.x, d.y), 0.0f) + length(max(d, 0.0f));
}

float SDF_Cone(in vec3 pos, in vec2 c, in float height) {
    // c is the sin/cos of the angle, h is height
    vec2 q = height * vec2(c.x / c.y, -1.0f);

    vec2 w = vec2(length(pos.xz), pos.y);
    vec2 a = w - q * clamp(dot(w,q) / dot(q, q), 0.0f, 1.0f);
    vec2 b = w - q * vec2(clamp(w.x / q.x, 0.0f, 1.0f), 1.0f);
    float k = sign(q.y);
    float d = min(dot(a, a), dot(b, b));
    float s = max(k * (w.x * q.y - w.y * q.x), k * (w.y - q.y));
    return sqrt(d) * sign(s);
}

float SDF_Torus(in vec3 pos, in float Radius, in float radius) {
    vec2 q = vec2(length(pos.xz) - Radius, pos.y);
    return length(q) - radius;
}

/* ---------------------------------------------------------------------------------------------- *
 *   TRANSFORMATIONS                                                                              *
 * ---------------------------------------------------------------------------------------------- */

mat2 rotation2D(in float angle) {
    float cosine = cos(angle);
    float sine = sin(angle);

    return mat2(cosine, -sine, sine, cosine);
}

vec3 rotation3D(in vec3 point, in vec3 axis, in float angle) {
    // Rodrigues' rotation formula
    return mix(dot(axis, point) * axis, point, cos(angle)) + cross(axis, point) * sin(angle);
}

/* ---------------------------------------------------------------------------------------------- *
 *   UTILITY FUNCTIONS                                                                            *
 * ---------------------------------------------------------------------------------------------- */

float smin(in float a, in float b, in float t) {
    float h = clamp(0.5f + 0.5f * (a - b) / t, 0.0f, 1.0f);
    return mix(a, b, h) - t * h * (1.0f - h);
}

float smax(in float a, in float b, in float t) {
    float h = clamp(0.5f - 0.5f * (a - b) / t, 0.0f, 1.0f);
    return mix(a, b, h) + t * h * (1.0f - h);
}

float min3(in float a, in float b, in float c) {
    return min(a, min(b, c));
}

float max3(in float a, in float b, in float c) {
    return max(a, max(b, c));
}

vec4 intersectSDF(vec4 a, vec4 b) {
    return a.w > b.w ? a : b;
}

vec4 unionSDF(vec4 a, vec4 b) {
    return a.w < b.w ? a : b;
}

vec4 differenceSDF(vec4 a, vec4 b) {
    return a.w > -b.w ? a : vec4(b.rgb, -b.w);
}

vec4 sIntersectSDF(vec4 a, vec4 b, in float t) {
    float h = clamp(0.5f - 0.5f * (a.w - b.w) / t, 0.0f, 1.0f);
    return vec4(mix(a.rgb, b.rgb, 1.0f - h), mix(a.w, b.w, h) + t * h * (1.0f - h));
}

vec4 sUnionSDF(vec4 a, vec4 b, in float t) {
    float h = clamp(0.5f + 0.5f * (a.w - b.w) / t, 0.0f, 1.0f);
    return vec4(mix(a.rgb, b.rgb, h), mix(a.w, b.w, h) - t * h * (1.0f - h));
}

vec4 sDifferenceSDF(vec4 a, vec4 b, in float t) {
    float h = clamp(0.5f - 0.5f * (a.w + b.w) / t, 0.0f, 1.0f);
    return vec4(mix(a.rgb, b.rgb, h), mix(a.w, -b.w, h) + t * h * (1.0f - h));
}

vec3 checker(in vec3 pos, in vec3 col1, in vec3 col2) {
    return mod(floor(pos.x) + floor(pos.z), 2.0f) == 0.0f ? col1 : col2;
}

/* ---------------------------------------------------------------------------------------------- *
 *   MAIN                                                                                         *
 * ---------------------------------------------------------------------------------------------- */

void main() {
    fragColor = vec4(renderAntiAliasing4(), 1.0f);
}
