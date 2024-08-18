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

vec3 phongLighting(in Ray ray, in vec3 pos) {
    const vec3 lightColor = vec3(1.0f);
    const vec3 normal = getNormal(pos);

    // Ambient Lighting
    float ambient = 0.2f;

    // Diffuse Lighting
    vec3 lightDirection = normalize(LIGHT_POSITION - pos);
    float diffuse = clamp(dot(normal, lightDirection), 0.0f, 1.0f);

    // Specular lighting
    vec3 reflectionDir = reflect(-lightDirection, normal);
    float specular = 0.25f * pow(max(dot(-ray.direction, reflectionDir), 0.0f), 32.0f);

    // Shadows
    float distance = raymarch(pos + normal * 0.02f, lightDirection);
    if(distance < length(LIGHT_POSITION - pos)) {
        return lightColor * ambient;
    }

    return lightColor * (ambient + diffuse + specular);
}
