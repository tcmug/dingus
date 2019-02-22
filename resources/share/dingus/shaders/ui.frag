#version 330

uniform sampler2D surface;
uniform vec3 tint;

in vec2 uv;

layout(location = 0) out vec4 color;

void main() {
    color = vec4(tint, 1) * texture(surface, uv);
}


// #version 330

// uniform sampler2D diffuse_texture;

// in vec3 clr;
// in vec2 uv;
// out vec4 color;

// void main() {

    
// 	color = texture(diffuse_texture, uv);
// }