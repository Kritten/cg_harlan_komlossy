#version 330


in vec4 passed_normal;
in vec3 passed_vs_position;
in vec2 passed_tex_coord;

uniform sampler2D ColorTexture0;
uniform sampler2D ColorTexture1;
uniform mat4 ViewMatrix;
uniform int ShadingMode; // 1: Normal Phong Shading   2: Cell Shading
uniform vec3 LightPosition;

out vec4 out_Color;


void main(void)
{
    vec3 texture_Color0 = texture2D(ColorTexture0, passed_tex_coord).xyz;
    vec3 texture_Color1 = texture2D(ColorTexture1, passed_tex_coord).xyz;
    
    vec3 total_Color = 2.7*texture_Color0 + 0.4*texture_Color1;

    out_Color = vec4(total_Color, 1.0);
}