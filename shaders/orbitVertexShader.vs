#version 330

layout(location=0) in vec3 in_Position;

out vec3 passed_vs_position;

//Matrix Uniforms as specified with glUniformMatrix4fv
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;


void main(void)
{
	vec4 vertexPos = vec4(in_Position, 1.0);

	vec4 vs_position = (ViewMatrix * ModelMatrix) * vertexPos;

	passed_vs_position = vec3(vs_position.xyz);

	gl_Position = ProjectionMatrix * vs_position;

}
