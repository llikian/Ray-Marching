/***************************************************************************************************
 * @file  lighting.glsl
 * @brief Implementation of functions regarding lighting
 **************************************************************************************************/

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
