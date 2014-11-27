#version 330

layout(location=0) in vec3 in_Position;
layout(location=1) in vec2 in_TexCoord;

out vec4 passed_normal;
out vec3 passed_vs_position;
out vec2 passed_tex_coord;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 NormalMatrix;


void main(void)
{
	vec4 vertexPos = vec4(in_Position, 1.0);
	
	vec4 vs_position = (ViewMatrix * ModelMatrix) * vertexPos;
	passed_vs_position = vec3(vs_position.xyz);
	
	passed_tex_coord = in_TexCoord;

	passed_normal = normalize(NormalMatrix * vec4(vertexPos.xyz , 0.0));
	passed_normal = vec4(normalize(passed_normal.xyz), 0.0);
	
	gl_Position = ProjectionMatrix * vs_position;
}
