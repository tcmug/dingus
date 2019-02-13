#version 330

uniform sampler2D glyph_texture;
in vec2 uv;

layout(location = 0) out vec4 color;

void main()
{
    color = texture(glyph_texture, uv);
}


// #version 330

// uniform sampler2D diffuse_texture;

// in vec3 clr;
// in vec2 uv;
// out vec4 color;

// void main() {

    
// 	color = texture(diffuse_texture, uv);
// }