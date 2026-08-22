#version 450

layout(location = 0) out vec3 nearPoint;
layout(location = 1) out vec3 farPoint;

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

vec3 gridPlane[6] = vec3[](
    vec3(-1.0, -1.0, 0.0), vec3(1.0, -1.0, 0.0), vec3(1.0, 1.0, 0.0),
    vec3(-1.0, -1.0, 0.0), vec3(1.0, 1.0, 0.0),  vec3(-1.0, 1.0, 0.0)
);

vec3 UnprojectPoint(float x, float y, float z, mat4 viewProjInv) {
    vec4 unprojectedPoint = viewProjInv * vec4(x, y, z, 1.0);
    return unprojectedPoint.xyz / unprojectedPoint.w;
}

void main() {
    vec3 p = gridPlane[gl_VertexIndex];
    mat4 viewProjInv = inverse(ubo.proj * ubo.view);

    nearPoint = UnprojectPoint(p.x, p.y, 0.0, viewProjInv);
    farPoint = UnprojectPoint(p.x, p.y, 1.0, viewProjInv);

    gl_Position = vec4(p, 1.0);
}
