#version 330

layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Normal;
out vec4 passed_normal;
out vec3 passed_vs_position;

//Matrix Uniforms as specified with glUniformMatrix4fv
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 NormalMatrix;


void main(void)
{
	vec4 vertexPos = vec4(in_Position, 1.0);
	vec4 vs_position = (ViewMatrix * ModelMatrix) * vertexPos;
	
	gl_Position = ProjectionMatrix * vs_position;

	passed_vs_position = vec3(vs_position.xyz);

	passed_normal = normalize(NormalMatrix * vec4(vertexPos.xyz , 0.0));
	// passed_normal = normalize(NormalMatrix * vec4(in_Normal, 0.0));
	
	passed_normal = vec4(normalize(passed_normal.xyz), 0.0);
}
