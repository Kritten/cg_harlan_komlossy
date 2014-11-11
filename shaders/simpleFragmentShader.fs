#version 330

uniform vec3 PlanetColor;

in  vec4 passed_normal;
in  vec3 passed_vs_position;

out vec4 out_Color;


void main(void)
{
 	vec3 vec_to_fragment = normalize(passed_vs_position);

    if (abs(dot(passed_normal.xyz, (-1.0)*vec_to_fragment ) ) < 0.4)
    {
    	out_Color = vec4(0.0, 0.0, 0.0, 1.0);
    }else{
    	out_Color = vec4(PlanetColor, 1.0);
    	// out_Color = vec4(abs(passed_normal.x), abs(passed_normal.y), abs(passed_normal.z), 1.0);
    }

}