/***************************************************************************************************
 * @file  lighting.glsl
 * @brief Implementation of functions regarding lighting
 **************************************************************************************************/

vec3 getNormal(in vec3 pos) {
    vec2 epsilon = vec2(0.001f, 0.0f);

    vec3 v1 = vec3(map(pos + epsilon.xyy).w, map(pos + epsilon.yxy).w, map(pos + epsilon.yyx).w);
    vec3 v2 = vec3(map(pos - epsilon.xyy).w, map(pos - epsilon.yxy).w, map(pos - epsilon.yyx).w);

    return normalize(v1 - v2);
}

vec3 phongLighting(in Ray ray, in vec3 pos) {
    const vec3 lightPos = LIGHT_POSITION;
    const vec3 lightColor = vec3(1.0f);
    const vec3 normal = getNormal(pos);

    // Ambient Lighting
    float ambient = 0.2f;

    // Diffuse Lighting
    vec3 lightDirection = normalize(lightPos - pos);
    float diffuse = max(dot(normal, lightDirection), 0.0f);

    // Specular lighting
    vec3 viewDirection = normalize(ray.origin - pos);
    vec3 reflectionDir = reflect(-lightDirection, normal);
    float specular = 0.25f * pow(max(dot(viewDirection, reflectionDir), 0.0f), 32.0f);

    // Shadows
    float distance = raymarch(pos + normal * MIN_DISTANCE * 2.0f, lightDirection);
    if(distance < length(lightPos - pos)) {
        return vec3(ambient);
    }

    return (ambient + diffuse + specular) * lightColor;
}