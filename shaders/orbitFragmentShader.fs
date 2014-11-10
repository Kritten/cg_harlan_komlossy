#version 330

out vec4 out_Color;
in vec3 passed_color;

void main(void)
{
    out_Color = vec4(passed_color, 1.0);
}
