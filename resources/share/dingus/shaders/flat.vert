#version 330

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texture_uv;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 nor;
out vec2 uv;

void main() {
    gl_Position = projection * view * model * vec4(vertex, 1);
    uv = texture_uv;
    mat4 model_normal = model;
    model_normal[3][0] = 0;
    model_normal[3][1] = 0;
    model_normal[3][2] = 0;
    nor = vec3(model_normal * vec4(normal, 1));
}

