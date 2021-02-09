#version 330 core

layout(location = 0) in vec4 in_Position;
layout(location = 1) in vec4 in_Normal;

layout(location = 2) in vec4 in_LightPos;
layout(location = 3) in vec4 in_LightAmbient;
layout(location = 4) in vec4 in_LightDiffuse;
layout(location = 5) in vec4 in_LightSpecular;

layout(location = 6) in vec4 in_MaterialEmissive;
layout(location = 7) in vec4 in_MaterialAmbient;
layout(location = 8) in vec4 in_MaterialDiffuse;
layout(location = 9) in vec4 in_MaterialSpecular;
layout(location = 10) in float in_MaterialShininess;

layout(location = 11) in vec4 in_PaintedColor;
layout(location = 12) in vec2 in_TexCoord;
layout(location = 13) in float in_LightPower;



out vec4 ex_Normal;
out vec3 ex_NormalInterp;

out vec3 ex_LightPos;
out vec4 ex_LightAmbient;
out vec4 ex_LightDiffuse;
out vec4 ex_LightSpecular;

out vec4 ex_MaterialEmissive;
out vec4 ex_MaterialAmbient;
out vec4 ex_MaterialDiffuse;
out vec4 ex_MaterialSpecular;
out float ex_MaterialShininess;

out vec4 ex_PaintedColor;

out vec3 ex_FragPos;
out vec2 ex_TexCoord;
out float ex_LightPower;


uniform mat4 ModelMatrix;

uniform SharedMatrices
{
    mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

mat4 NormalMatrix = mat4(
    1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0
);

void main(void)
{
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * in_Position;

	// just send to fragment shader
	ex_Normal = in_Normal;
    ex_LightAmbient = in_LightAmbient;
    ex_LightDiffuse = in_LightDiffuse;
    ex_LightSpecular = in_LightSpecular;
    ex_MaterialEmissive = in_MaterialEmissive;
    ex_MaterialAmbient = in_MaterialAmbient;
    ex_MaterialDiffuse = in_MaterialDiffuse;
    ex_MaterialSpecular = in_MaterialSpecular;
    ex_MaterialShininess = in_MaterialShininess;
    ex_PaintedColor = in_PaintedColor;
    ex_TexCoord = in_TexCoord;
    ex_LightPower = in_LightPower;

	// get fragment position to fragment shader
    vec4 vertPos4 = ViewMatrix * ModelMatrix * in_Position;
	ex_FragPos = vec3(vertPos4) / vertPos4.w;

    ex_NormalInterp = vec3(NormalMatrix * vec4(vec3(in_Normal), 0.0));

    vec4 LightPos4 = ViewMatrix * in_LightPos;
    ex_LightPos = vec3(LightPos4) / LightPos4.w;
}