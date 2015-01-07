#version 330

uniform ivec2 ScreenDimensions;

uniform sampler2D ColorTexture;

layout (location = 0) out vec4 out_Color;

void main(void)
{
	vec2 tex_coords = gl_FragCoord.xy / ScreenDimensions;

	vec3 color = texture2D(ColorTexture, tex_coords ).rgb;
    out_Color = vec4(color, 1.0);
}