#version 330

layout(location=0) in vec3 in_Position;

out vec3 passed_ws_position;
out vec3 passed_vs_position;

//Matrix Uniforms as specified with glUniformMatrix4fv
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;


void main(void)
{
	vec4 vertexPos = vec4(in_Position, 1.0);

	vec4 ws_position = ModelMatrix * vertexPos;
	passed_ws_position = vec3(ws_position.xyz) / ws_position.w;

	vec4 vs_position = ViewMatrix * ws_position;
	passed_vs_position = vec3(vs_position.xyz) / vs_position.w;

	gl_Position = ProjectionMatrix * vs_position;

}
