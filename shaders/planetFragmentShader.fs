#version 330


in vec4 passed_normal;
in vec3 passed_vs_position;
in vec3 passed_ws_position;
in vec2 passed_tex_coord;
in vec3 passed_tangent;
in vec3 passed_bitangent;

uniform sampler2D ColorTexture;
uniform sampler2D NormalMapTexture;
uniform sampler2D GlossMapTexture;
uniform sampler2D SpecularMapTexture;

uniform mat4 NormalMatrix;

layout(location=0) out vec4 out_Color;
layout(location=1) out vec4 out_Normal;
layout(location=2) out vec4 out_Position;


void main(void)
{
    vec3 tangent = normalize(vec3((NormalMatrix * vec4(passed_tangent, 0)).xyz)); 
    vec3 bitangent = normalize(vec3((NormalMatrix * vec4(passed_bitangent, 0)).xyz)); 
 
    mat3 tangentSpaceMat = mat3(tangent, bitangent, passed_normal);
 
    vec3 texture_Normal = texture2D(NormalMapTexture, passed_tex_coord).rgb;
    vec3 finished_Normal = normalize(tangentSpaceMat * normalize(texture_Normal * vec3(2, 2, 1) - vec3(1, 1, 0)));

    vec3 texture_Color = texture2D(ColorTexture, passed_tex_coord).xyz; //colormap
    float texture_Gloss = texture2D(GlossMapTexture, passed_tex_coord).x; //glossmap
    vec3 texture_Specular = texture2D(SpecularMapTexture, passed_tex_coord).xyz; //specularmap
    float texture_Specular_exponent = (texture_Specular.x + texture_Specular.y + texture_Specular.z) / 3.0f ;

    out_Color = vec4(texture_Color, 1.0);
    out_Normal = vec4(finished_Normal, 1.0);;
    out_Position = vec4(passed_ws_position, 1.0);
}