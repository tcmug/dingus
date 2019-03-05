#version 330

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec2 texture_uv;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 uv;

void main() {
    gl_Position = projection * view * model * vec4(vertex, 1);
    uv = texture_uv;
}

