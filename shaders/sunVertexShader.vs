#version 330

layout(location=0) in vec3 in_Position;
layout(location=1) in vec2 in_TexCoord;

out vec4 passed_normal;
out vec3 passed_vs_position;
out vec3 passed_ws_position;
out vec2 passed_tex_coord;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 NormalMatrix;
uniform float Time;

uniform sampler2D ColorTexture0;


void main(void)
{
	vec2 tex = vec2(in_TexCoord.x + Time*0.1, in_TexCoord.y+ Time*0.2);
	vec3 texture_Specular = texture2D(ColorTexture0, tex).xyz; //specularmapa
	float texture_Specular_exponent = (texture_Specular.x + texture_Specular.y + texture_Specular.z) / 10.0f ;

	vec3 tmp_position = in_Position + texture_Specular_exponent * in_Position;

	vec4 vertexPos = vec4(tmp_position, 1.0);
	
	vec4 ws_position = ModelMatrix * vertexPos;
	passed_ws_position = vec3(ws_position.xyz) / ws_position.w;

	vec4 vs_position = ViewMatrix * ws_position;
	passed_vs_position = vec3(vs_position.xyz) / vs_position.w;
	
	passed_tex_coord = in_TexCoord;

	passed_normal = normalize(NormalMatrix * vec4(vertexPos.xyz , 0.0));
	passed_normal = vec4(normalize(passed_normal.xyz), 0.0);
	
	gl_Position = ProjectionMatrix * vs_position;
}
