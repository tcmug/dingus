#version 330

uniform sampler2D surface;

in vec2 uv;

layout(location = 0) out vec4 color;

void main() {
    color = vec4(1,1,1,1) * texture(surface, uv);
}
