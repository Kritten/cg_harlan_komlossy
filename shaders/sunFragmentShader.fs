#version 330


in vec4 passed_normal;
in vec3 passed_vs_position;
in vec2 passed_tex_coord;

uniform sampler2D ColorTexture;
uniform mat4 ViewMatrix;
uniform int ShadingMode; // 1: Normal Phong Shading   2: Cell Shading
uniform vec3 LightPosition;

out vec4 out_Color;


void main(void)
{
    
    vec3 texture_Color = texture2D(ColorTexture, passed_tex_coord).xyz;
    out_Color = vec4(5*texture_Color, 1.0);
}