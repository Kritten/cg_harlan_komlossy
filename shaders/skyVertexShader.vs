#version 330

layout(location=0) in vec3 in_Position;
layout(location=1) in vec2 in_TexCoord;

out vec2 passed_tex_coord;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;


void main(void)
{
	vec4 vertexPos = vec4(in_Position, 1.0);
	
	passed_tex_coord = in_TexCoord;

	gl_Position = ( ProjectionMatrix * ViewMatrix * ModelMatrix ) * vertexPos;
}
