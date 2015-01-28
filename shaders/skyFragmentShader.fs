#version 330

in vec2 passed_tex_coord;

uniform sampler2D ColorTexture;

layout(location=0) out vec4 out_Color;
layout(location=1) out vec4 out_Normal;
layout(location=2) out vec4 out_Position;


void main(void)
{
	vec3 textureColor = texture2D(ColorTexture, passed_tex_coord).xyz;
	textureColor = 0.5 * textureColor;
	out_Position = vec4(0.0, 0.0, 0.0, 0.0);
	out_Normal = vec4(0.0, 0.0, 0.0, 0.0);
    out_Color = vec4(textureColor, 1.0);
}