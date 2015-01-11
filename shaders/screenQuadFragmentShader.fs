#version 330

uniform ivec2 ScreenDimensions;
uniform int greyscale;
uniform int horizontal_mirrowed;
uniform int vertical_mirrowed;
uniform int blur;

uniform sampler2D ColorTexture;

layout (location = 0) out vec4 out_Color;

float gaussian_weights[9] = float[9] //example for 3x3 weights in gauss kernel
							(1.0/16.0, 1.0/ 8.0, 1.0/16.0,
							1.0/ 8.0, 1.0/ 4.0, 1.0/ 8.0,
							1.0/16.0, 1.0/ 8.0, 1.0/16.0);


void main(void)
{

	vec2 tex_coords = (gl_FragCoord.xy) / ScreenDimensions;

	if(horizontal_mirrowed == 1)
	{
		tex_coords = vec2(1.0-tex_coords.x, tex_coords.y);
		
	}
	if(vertical_mirrowed == 1)
	{
		tex_coords = vec2(tex_coords.x, 1.0-tex_coords.y);
		
	}

	vec3 texture_color = texture2D(ColorTexture, tex_coords ).rgb;
	vec3 color = vec3(texture_color);

	if(blur == 1)
	{
		vec4 blurred_color = vec4(0.0);
		for(int y = -1; y <= 1; y++)
		{
			for(int x = -1; x <= 1; x++)
			{
				tex_coords *= ScreenDimensions;
				tex_coords = vec2(tex_coords.x + x, tex_coords.y + y) / ScreenDimensions;
				vec3 tex_color = texture2D(ColorTexture, tex_coords).rgb;

				int z = 0;	
				if(y == -1) 
				{
					z = 1;
				}	
				else if(y == 0)
				{
					z = 4;
				}
				else
				{
					z = 7;
				}
				blurred_color += vec4(gaussian_weights[z+x] * tex_color, gaussian_weights[z+x]);
			}
		}
		color = vec3(blurred_color.xyz/blurred_color.w);
	}

	if(greyscale == 1)
	{
		float grey_value = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
		color = vec3(grey_value);
	}
    out_Color = vec4(color, 1.0);
}