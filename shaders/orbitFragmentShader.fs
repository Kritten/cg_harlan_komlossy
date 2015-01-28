#version 330
uniform vec3 OrbitColor;

// G-Buffer Layout
layout(location=0) out vec4 out_Color;
layout(location=1) out vec4 out_Normal;
layout(location=2) out vec4 out_Position;

in vec3 passed_ws_position;
in vec3 passed_vs_position;

void main(void)
{    
    out_Color = vec4(OrbitColor, 1.0);
    out_Normal = vec4(0.0, 0.0, 0.0, 0.0);
    out_Position = vec4(passed_ws_position, 0.0);
}


