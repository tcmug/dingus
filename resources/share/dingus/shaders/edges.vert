#version 330

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texture_uv;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
    vec4 v = view * model * vec4(vertex, 1);
    vec3 edge = normal * 0.05;
    //(-v.z * 0.0025);
    vec3 ve = vertex + edge;
    gl_Position = projection * view * model * vec4(ve, 1);
}

