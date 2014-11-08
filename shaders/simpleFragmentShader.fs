#version 330

in  vec4 passed_normal;

out vec4 out_Color;


void main(void)
{
    out_Color = normalize(passed_normal+0.3);
}
