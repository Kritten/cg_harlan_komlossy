#version 330

uniform ivec2 ScreenDimensions;
uniform int greyscale;
uniform int horizontal_mirrowed;
uniform int vertical_mirrowed;
uniform int blur;

uniform sampler2D ColorTexture;
uniform sampler2D NormalTexture;
uniform sampler2D PositionTexture;

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
				vec2 new_tex_coords = tex_coords * ScreenDimensions;
				new_tex_coords = vec2(new_tex_coords.x + x, new_tex_coords.y + y) / ScreenDimensions;
				vec3 tex_color = texture2D(ColorTexture, new_tex_coords).rgb;

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



// ORBIT SHADER

// float ka = 0.0f;
// float sun_inten = 1.0f;
// float light_inten = 1.0f;

// float distance_func(vec3 vector_1, vec3 vector_2, float faktor)
// {
// 	return faktor/length(vector_2 - vector_1);
// }

// void main(void)
// {    
// 	vec4 sun_location = ViewMatrix*vec4(0.0, 0.0, 0.0, 1.0);

//     vec4 light_location = ViewMatrix*vec4(LightPosition, 1.0);
//     float total = 0.0;

//     float sun_part = distance_func(sun_location.xyz, passed_vs_position, 3.0f) * sun_inten;
//     float light_part = distance_func(light_location.xyz, passed_vs_position, 0.2f) * light_inten;

//     total = ka + sun_part + light_part;
//     out_Color = vec4(OrbitColor, 1.0);
//     out_Normal = vec4(0.0, 0.0, 0.0, 0.0);
//     out_Position = vec4(passed_ws_position, 0.0);
// }


// Planet Shader

// // Globals
// float ka = 0.06f;
// float sun_kd = 0.5f;
// float sun_ks = 2.0f;
// float light_kd = 0.5f;
// float light_ks = 2.0f;
// int n = 30;

// float distance_func(vec3 vector_1, vec3 vector_2, float faktor)
// {
//     return faktor/length(vector_2 - vector_1);
// }

// float cell_shading_remap(float value)
// {
//     if (value < 0.3)
//     {
//         return 0.2;
//     }
//     if (value < 0.7)
//     {
//         return 0.5;
//     }
//     if (value > 1.5)
//     {
//         return 10.0;
//     }
//     return 1.0;
// }

// void main(void)
// {
//     float total = 0.0;
//     vec3 vec_to_fragment = normalize(passed_vs_position);


//     if (ShadingMode == 2)
//     {
//         if (abs(dot(passed_normal.xyz, (-1.0)*vec_to_fragment ) ) < 0.3)
//         {
//             out_Color = vec4(0.0, 0.0, 0.0, 1.0);
//             return;
//         }
//     }

    
//     vec3 tangent = normalize(vec3((NormalMatrix * vec4(passed_tangent, 0)).xyz)); 
//     vec3 bitangent = normalize(vec3((NormalMatrix * vec4(passed_bitangent, 0)).xyz)); 
//     mat3 tangentSpace = mat3(tangent, bitangent, passed_normal);
//     vec3 texture_Normal = normalize(tangentSpace * normalize(texture2D(NormalMapTexture, passed_tex_coord).rgb * vec3(2, 2, 1) - vec3(1, 1, 0)));


//     vec3 texture_Color = texture2D(ColorTexture, passed_tex_coord).xyz; //colormap
//     // vec3 texture_Normal = texture2D(NormalMapTexture, passed_tex_coord).xyz; //normalmap
//     float texture_Gloss = texture2D(GlossMapTexture, passed_tex_coord).x; //glossmap
//     vec3 texture_Specular = texture2D(SpecularMapTexture, passed_tex_coord).xyz; //specularmap
//     float texture_Specular_exponent = (texture_Specular.x + texture_Specular.y + texture_Specular.z) / 3.0f ;

//     float ambient  = ka;


//     // SUN LIGHTING
//     vec4 sun_location = ViewMatrix*vec4(0.0, 0.0, 0.0, 1.0);
//     vec3 vertex_to_sun = normalize(sun_location.xyz-passed_vs_position);

//     // Diffus
//     float dot_product_diffus = dot(texture_Normal, vertex_to_sun);
//     float diffus = sun_kd * max(0.0, dot_product_diffus);

//     // Specular
//     float dot_product_specular = dot(normalize(reflect(vertex_to_sun, texture_Normal)), normalize(passed_vs_position)); 
//     // float specular = sun_ks * max(0.0, pow(dot_product_specular, n));
//     float specular = sun_ks * texture_Gloss * max(0.0, pow(dot_product_specular, texture_Specular_exponent*n));

//     float total_sun = distance_func(sun_location.xyz, passed_vs_position, 8.0f) * (diffus + specular);


//     // LIGHTSOURCE LIGHTING 
//     vec4 light_location = ViewMatrix*vec4(LightPosition, 1.0);
//     vec3 vertex_to_light = normalize(light_location.xyz-passed_vs_position);

//     // diffus 
//     dot_product_diffus = dot(texture_Normal, vertex_to_light);
//     diffus   = light_kd * max(0.0, pow(specular, n));

//     // specular
//     dot_product_specular = dot(normalize(reflect(vertex_to_light, texture_Normal)), normalize(passed_vs_position)); 
//     // specular = light_ks * max(0.0, pow(dot_product_specular, n));
//     specular = light_ks * texture_Gloss * max(0.0, pow(dot_product_specular, texture_Specular_exponent*n));

//     float total_lightsource = distance_func(light_location.xyz, passed_vs_position, 2.0f) * (diffus + specular);
    

//     // TOTAL LIGHTING
//     total = ambient + total_sun + total_lightsource;
   
//     out_Normal = passed_normal;
//     out_Position = vec4(passed_ws_position, 1.0);

//     if (ShadingMode == 1)
//     {
//         out_Color = vec4(texture_Color * total, 1.0);
//         return;
//     }

//     if (ShadingMode == 2)
//     {
//         total = cell_shading_remap(total);
//         out_Color = vec4(texture_Color * total, 1.0);
//         return;
//     }

//     out_Color = vec4(1.0, 0.0, 0.0, 1.0);
// }