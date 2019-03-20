#version 330

uniform sampler2D surface;

in vec3 nor;
in vec2 uv;

layout(location = 0) out vec4 color;

void main() {
    float col = max(0.1, dot(nor, vec3(1, 0, 0)));
    color = vec4(col, col, col, 1);// * texture(surface, uv);
}
