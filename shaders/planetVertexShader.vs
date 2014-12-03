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

uniform sampler2D ColorTexture;



void main(void)
{
    vec3 texture_Specular = texture2D(ColorTexture, in_TexCoord).xyz; //specularmapa
    float texture_Specular_exponent = (texture_Specular.x + texture_Specular.y + texture_Specular.z) / 60.0f ;

    vec3 tmp_position = in_Position + texture_Specular_exponent * in_Position;
    // vec3 tmp_position = in_Position;

	vec4 vertexPos = vec4(tmp_position, 1.0);
		
	vec4 vs_position = (ViewMatrix * ModelMatrix) * vertexPos;
	passed_vs_position = vec3(vs_position.xyz);
	
	passed_tex_coord = in_TexCoord;

	passed_normal = normalize(NormalMatrix * vec4(vertexPos.xyz , 0.0));
	passed_normal = vec4(normalize(passed_normal.xyz), 0.0);
	
	gl_Position = ProjectionMatrix * vs_position;
}
