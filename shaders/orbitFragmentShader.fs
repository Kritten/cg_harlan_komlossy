#version 330

uniform mat4 ViewMatrix;
uniform vec3 OrbitColor;
uniform vec3 LightPosition;

out vec4 out_Color;
in  vec3 passed_vs_position;

float ka = 0.0f;
float sun_inten = 1.0f;
float light_inten = 1.0f;

float distance_func(vec3 vector_1, vec3 vector_2, float faktor)
{
	return faktor/length(vector_2 - vector_1);
}

void main(void)
{    
	vec4 sun_location = ViewMatrix*vec4(0.0, 0.0, 0.0, 1.0);

    vec4 light_location = ViewMatrix*vec4(LightPosition, 1.0);
    float total = 0.0;

    float sun_part = distance_func(sun_location.xyz, passed_vs_position, 3.0f) * sun_inten;
    float light_part = distance_func(light_location.xyz, passed_vs_position, 0.2f) * light_inten;

    total = ka + sun_part + light_part;
    out_Color = vec4(total * OrbitColor, 1.0);
}


