#version 460 core

const vec3 RED = vec3(1.0f, 0.0f, 0.0f);
const vec3 BLUE = vec3(0.0f, 0.0f, 1.0f);

float SDF_Box(in vec3 pos, in vec3 dimensions);
vec4 intersectSDF(vec4 a, vec4 b);
vec4 sIntersectSDF(vec4 a, vec4 b, in float t);
vec4 unionSDF(vec4 a, vec4 b);
vec4 sUnionSDF(vec4 a, vec4 b, in float t);
vec4 differenceSDF(vec4 a, vec4 b);
vec4 sDifferenceSDF(vec4 a, vec4 b, in float t);

vec4 map(in vec3 pos) {
    vec3 p;

    vec4 box = vec4(RED, 0.0f);
    vec4 beams = vec4(BLUE, 0.0f);

    float distance = 8.0f;
    float smoothing = 1.0f;

    vec4 result;

    p = pos - vec3(distance, -2.0f * -distance, 0.0f);
    box.w = SDF_Box(p, vec3(2.0f));
    result = box;

    p = pos - vec3(-distance, -2.0f * -distance, 0.0f);
    beams.w = SDF_Box(p, vec3(1.0f, 3.0f, 1.0f));
    beams.w = min(beams.w, SDF_Box(p, vec3(3.0f, 1.0f, 1.0f)));
    beams.w = min(beams.w, SDF_Box(p, vec3(1.0f, 1.0f, 3.0f)));
    result = unionSDF(result, beams);

    p = pos - vec3(-distance, 0.0f, 0.0f);
    box.w = SDF_Box(p, vec3(2.0f));
    beams.w = SDF_Box(p, vec3(1.0f, 3.0f, 1.0f));
    beams.w = min(beams.w, SDF_Box(p, vec3(3.0f, 1.0f, 1.0f)));
    beams.w = min(beams.w, SDF_Box(p, vec3(1.0f, 1.0f, 3.0f)));
    result = unionSDF(result, unionSDF(box, beams));

    p = pos;
    box.w = SDF_Box(p, vec3(2.0f));
    beams.w = SDF_Box(p, vec3(1.0f, 3.0f, 1.0f));
    beams.w = min(beams.w, SDF_Box(p, vec3(3.0f, 1.0f, 1.0f)));
    beams.w = min(beams.w, SDF_Box(p, vec3(1.0f, 1.0f, 3.0f)));
    result = unionSDF(result, intersectSDF(box, beams));

    p = pos - vec3(distance, 0.0f, 0.0f);
    box.w = SDF_Box(p, vec3(2.0f));
    beams.w = SDF_Box(p, vec3(1.0f, 3.0f, 1.0f));
    beams.w = min(beams.w, SDF_Box(p, vec3(3.0f, 1.0f, 1.0f)));
    beams.w = min(beams.w, SDF_Box(p, vec3(1.0f, 1.0f, 3.0f)));
    result = unionSDF(result, differenceSDF(box, beams));

    p = pos - vec3(-distance, distance, 0.0f);
    box.w = SDF_Box(p, vec3(2.0f));
    beams.w = SDF_Box(p, vec3(1.0f, 3.0f, 1.0f));
    beams.w = min(beams.w, SDF_Box(p, vec3(3.0f, 1.0f, 1.0f)));
    beams.w = min(beams.w, SDF_Box(p, vec3(1.0f, 1.0f, 3.0f)));
    result = unionSDF(result, sUnionSDF(box, beams, smoothing));

    p = pos - vec3(0.0f, distance, 0.0f);
    box.w = SDF_Box(p, vec3(2.0f));
    beams.w = SDF_Box(p, vec3(1.0f, 3.0f, 1.0f));
    beams.w = min(beams.w, SDF_Box(p, vec3(3.0f, 1.0f, 1.0f)));
    beams.w = min(beams.w, SDF_Box(p, vec3(1.0f, 1.0f, 3.0f)));
    result = unionSDF(result, sIntersectSDF(box, beams, smoothing));

    p = pos - vec3(distance, distance, 0.0f);
    box.w = SDF_Box(p, vec3(2.0f));
    beams.w = SDF_Box(p, vec3(1.0f, 3.0f, 1.0f));
    beams.w = min(beams.w, SDF_Box(p, vec3(3.0f, 1.0f, 1.0f)));
    beams.w = min(beams.w, SDF_Box(p, vec3(1.0f, 1.0f, 3.0f)));
    result = unionSDF(result, sDifferenceSDF(box, beams, smoothing));

    return result;
}
