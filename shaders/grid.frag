#version 450

layout(location = 0) in vec3 nearPoint;
layout(location = 1) in vec3 farPoint;

layout(location = 0) out vec4 outColor;

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

vec4 drawGrid(vec3 pos, float scale) {
    vec2 coord = pos.xz * scale;
    vec2 derivative = fwidth(coord);
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
    float line = min(grid.x, grid.y);
    float minimumz = min(derivative.y, 1.0);
    float minimumx = min(derivative.x, 1.0);

    vec4 color = vec4(0.2, 0.2, 0.2, 1.0 - min(line, 1.0));

    // Highlight world axes
    if (pos.x > -0.1 * minimumx && pos.x < 0.1 * minimumx) color.z = 1.0; // Z Axis (Blue)
    if (pos.z > -0.1 * minimumz && pos.z < 0.1 * minimumz) color.x = 1.0; // X Axis (Red)

    return color;
}

float computeDepth(vec3 pos) {
    vec4 clip_space_pos = ubo.proj * ubo.view * vec4(pos, 1.0);
    return clip_space_pos.z / clip_space_pos.w;
}

float computeLinearDepth(vec3 pos) {
    vec4 clip_space_pos = ubo.proj * ubo.view * vec4(pos, 1.0);
    float clip_space_depth = clip_space_pos.z / clip_space_pos.w;
    float near = 0.1;
    float far = 10.0; // Match camera far plane
    return (near * far) / (far - clip_space_depth * (far - near));
}

void main() {
    // Solve for t where world point Y = 0: P = near + t * (far - near)
    float t = -nearPoint.y / (farPoint.y - nearPoint.y);

    // Discard if ray doesn't hit ground or points away from camera
    if (t < 0.0) discard;

    vec3 fragPos3D = nearPoint + t * (farPoint - nearPoint);

    // Compute proper depth writing to integrate with scene geometry
    gl_FragDepth = computeDepth(fragPos3D);

    // Grid anti-aliased patterns (major and minor lines)
    vec4 gridColor = drawGrid(fragPos3D, 1.0) + drawGrid(fragPos3D, 0.1);

    // Smooth horizon fade based on linear distance
    float linearDepth = computeLinearDepth(fragPos3D);
    // Normalized depth range [0.0, 1.0] relative to your camera (0.1 to 10.0)
    float linearDepthNorm = (linearDepth - 0.1) / (10.0 - 0.1);
    float fading = max(0.0, 1.0 - linearDepthNorm); // Fade out as it gets closer to 10.0

    outColor = gridColor;
    outColor.a *= fading;
}