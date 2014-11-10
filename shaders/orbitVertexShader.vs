#version 330

layout(location=0) in vec3 in_Position;

out vec3 passed_color;

//Matrix Uniforms as specified with glUniformMatrix4fv
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform vec3 OrbitColor;


void main(void)
{
	vec4 vertexPos = vec4(in_Position, 1.0);
	vertexPos = (ProjectionMatrix  * ViewMatrix * ModelMatrix) * vertexPos;
	passed_color = OrbitColor;
	gl_Position = vertexPos;

}
