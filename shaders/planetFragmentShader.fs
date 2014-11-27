#version 330


in vec4 passed_normal;
in vec3 passed_vs_position;
in vec2 passed_tex_coord;

uniform sampler2D ColorTexture;
uniform mat4 ViewMatrix;
uniform int ShadingMode; // 1: Normal Phong Shading   2: Cell Shading
uniform vec3 LightPosition;

out vec4 out_Color;


// Globals
float ka = 0.06f;
float sun_kd = 0.5f;
float sun_ks = 1.0f;
float light_kd = 0.5f;
float light_ks = 1.0f;
int n = 15;

float distance_func(vec3 vector_1, vec3 vector_2, float faktor)
{
	return faktor/length(vector_2 - vector_1);
}

float cell_shading_remap(float value)
{
    if (value < 0.3)
    {
        return 0.2;
    }
    if (value < 0.7)
    {
        return 0.5;
    }
    if (value > 1.5)
    {
        return 10.0;
    }
    return 1.0;
}

void main(void)
{
    float total = 0.0;
    vec3 vec_to_fragment = normalize(passed_vs_position);

    if (ShadingMode == 2)
    {
        if (abs(dot(passed_normal.xyz, (-1.0)*vec_to_fragment ) ) < 0.3)
        {
            out_Color = vec4(0.0, 0.0, 0.0, 1.0);
            return;
        }
    }

    float ambient  = ka;

    vec4 sun_location = ViewMatrix*vec4(0.0, 0.0, 0.0, 1.0);
    vec3 vertex_to_sun = normalize(sun_location.xyz-passed_vs_position);
    float dot_product_diffus = dot(passed_normal.xyz, vertex_to_sun);
    float dot_product_specular = dot(normalize(reflect(vertex_to_sun, passed_normal.xyz)), normalize(passed_vs_position)); 
    float diffus   = sun_kd * max(0.0, dot_product_diffus);
    float specular = sun_ks * max(0.0, dot_product_specular);
    float total_sun = distance_func(sun_location.xyz, passed_vs_position, 5.0f) * (diffus + pow(specular, n));

    vec4 light_location = ViewMatrix*vec4(LightPosition, 1.0);
    vec3 vertex_to_light = normalize(light_location.xyz-passed_vs_position);
    dot_product_diffus = dot(passed_normal.xyz, vertex_to_light);
    dot_product_specular = dot(normalize(reflect(vertex_to_light, passed_normal.xyz)), normalize(passed_vs_position)); 
    diffus   = light_kd * max(0.0, dot_product_diffus);
    specular = light_ks * max(0.0, dot_product_specular);
    float total_lightsource = distance_func(light_location.xyz, passed_vs_position, 2.0f) * (diffus + pow(specular, n));

    total = ambient + total_sun + total_lightsource;
    
    vec3 texture_Color = texture2D(ColorTexture, passed_tex_coord).xyz;

    if (ShadingMode == 1)
    {
        out_Color = vec4(texture_Color * total, 1.0);
        return;
    }

    if (ShadingMode == 2)
    {
        {
            total = cell_shading_remap(total);
            out_Color = vec4(texture_Color * total, 1.0);
            return;
        }
    }

    out_Color = vec4(1.0, 0.0, 0.0, 1.0);
}