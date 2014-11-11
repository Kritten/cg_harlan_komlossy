#version 330

float distance_func(vec3 vector_1, vec3 vector_2);

uniform vec3 PlanetColor;
uniform mat4 ViewMatrix;

in  vec4 passed_normal;
in  vec3 passed_vs_position;

out vec4 out_Color;

float ka = 0.2f;
float kd = 0.5f;
float ks = 1.0f;
int n = 15;


void main(void)
{
 	vec3 vec_to_fragment = normalize(passed_vs_position);

    // if (abs(dot(passed_normal.xyz, (-1.0)*vec_to_fragment ) ) < 0.4)
    // {
    // 	out_Color = vec4(0.0, 0.0, 0.0, 1.0);
    // }
    // else
    {
    	vec4 sun_location = ViewMatrix*vec4(0.0, 0.0, 0.0, 1.0);
    	vec3 vertex_to_sun = normalize(sun_location.xyz-passed_vs_position);
    	float dot_product_diffus = dot(passed_normal.xyz, vertex_to_sun);
    	float dot_product_specular = dot(normalize(reflect(vertex_to_sun, passed_normal.xyz)), normalize(passed_vs_position)); 
    	vec3 ambient  = ka * PlanetColor;
    	float diffus   = kd * max(0.0, dot_product_diffus);
    	float specular = ks * max(0.0, dot_product_specular);
    	vec3 total    = ambient + PlanetColor * distance_func(sun_location.xyz, passed_vs_position) * (diffus + pow(specular, n));
    	out_Color     = vec4(total, 1.0);
    	// out_Color = vec4(abs(passed_normal.x), abs(passed_normal.y), abs(passed_normal.z), 1.0);
    }

}

float distance_func(vec3 vector_1, vec3 vector_2)
{
	return 5.0f/length(vector_2 - vector_1);
}