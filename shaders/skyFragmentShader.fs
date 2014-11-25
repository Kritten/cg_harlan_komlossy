#version 330

uniform vec3 SkySphereColor;

out vec4 out_Color;


void main(void)
{
    out_Color = vec4(SkySphereColor, 1.0);
}