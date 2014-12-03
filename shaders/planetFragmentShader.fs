#version 330


in vec4 passed_normal;
in vec3 passed_vs_position;
in vec2 passed_tex_coord;

uniform sampler2D ColorTexture;
uniform sampler2D NormalMapTexture;
uniform sampler2D GlossMapTexture;
uniform sampler2D SpecularMapTexture;

uniform mat4 ViewMatrix;
uniform int ShadingMode; // 1: Normal Phong Shading   2: Cell Shading
uniform vec3 LightPosition;

out vec4 out_Color;


// Globals
float ka = 0.06f;
float sun_kd = 0.5f;
float sun_ks = 2.0f;
float light_kd = 0.5f;
float light_ks = 2.0f;
int n = 30;

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


    vec3 texture_Color = texture2D(ColorTexture, passed_tex_coord).xyz; //colormap
    vec3 texture_Normal = texture2D(NormalMapTexture, passed_tex_coord).xyz; //normalmap
    float texture_Gloss = texture2D(GlossMapTexture, passed_tex_coord).x; //glossmap
    vec3 texture_Specular = texture2D(SpecularMapTexture, passed_tex_coord).xyz; //specularmap
    float texture_Specular_exponent = (texture_Specular.x + texture_Specular.y + texture_Specular.z) / 3.0f ;

    float ambient  = ka;


    // SUN LIGHTING
    vec4 sun_location = ViewMatrix*vec4(0.0, 0.0, 0.0, 1.0);
    vec3 vertex_to_sun = normalize(sun_location.xyz-passed_vs_position);

    // Diffus
    float dot_product_diffus = dot(passed_normal.xyz, vertex_to_sun);
    float diffus = sun_kd * max(0.0, dot_product_diffus);

    // Specular
    float dot_product_specular = dot(normalize(reflect(vertex_to_sun, passed_normal.xyz)), normalize(passed_vs_position)); 
    float specular = sun_ks * texture_Gloss * max(0.0, pow(dot_product_specular, texture_Specular_exponent*n));

    float total_sun = distance_func(sun_location.xyz, passed_vs_position, 8.0f) * (diffus + specular);


    // LIGHTSOURCE LIGHTING 
    vec4 light_location = ViewMatrix*vec4(LightPosition, 1.0);
    vec3 vertex_to_light = normalize(light_location.xyz-passed_vs_position);

    // diffus 
    dot_product_diffus = dot(passed_normal.xyz, vertex_to_light);
    diffus   = light_kd * max(0.0, pow(specular, n));

    // specular
    dot_product_specular = dot(normalize(reflect(vertex_to_light, passed_normal.xyz)), normalize(passed_vs_position)); 
    specular = light_ks * texture_Gloss * max(0.0, pow(dot_product_specular, texture_Specular_exponent*n));

    float total_lightsource = distance_func(light_location.xyz, passed_vs_position, 2.0f) * (diffus + specular);
    

    // TOTAL LIGHTING
    total = ambient + total_sun + total_lightsource;
   

    if (ShadingMode == 1)
    {
        out_Color = vec4(texture_Color * total, 1.0);
        return;
    }

    if (ShadingMode == 2)
    {
        total = cell_shading_remap(total);
        out_Color = vec4(texture_Color * total, 1.0);
        return;
    }

    out_Color = vec4(1.0, 0.0, 0.0, 1.0);
}