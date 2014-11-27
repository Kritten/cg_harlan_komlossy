#version 330

in vec2 passed_tex_coord;

uniform sampler2D ColorTexture;

out vec4 out_Color;


void main(void)
{
	vec3 textureColor = texture2D(ColorTexture, passed_tex_coord).xyz;
	textureColor = 0.3 * textureColor;
    out_Color = vec4(textureColor, 1.0);
}