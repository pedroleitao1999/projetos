#version 330 core

in vec4 ex_Normal;
in vec3 ex_NormalInterp;

in vec3 ex_LightPos;
in vec4 ex_LightAmbient;
in vec4 ex_LightDiffuse;
in vec4 ex_LightSpecular;

in vec4 ex_MaterialEmissive;
in vec4 ex_MaterialAmbient;
in vec4 ex_MaterialDiffuse;
in vec4 ex_MaterialSpecular;
in float ex_MaterialShininess;

in vec4 ex_PaintedColor;

in vec3 ex_FragPos;

in vec2 ex_TexCoord;

in float ex_LightPower;

out vec4 out_Color;

out vec4 BrightColor;

uniform sampler2D in_Texture;
uniform sampler2D in_NormalMap;
uniform vec4 in_LightColor;
uniform vec3 in_LightPos;

vec3 normal = normalize(ex_NormalInterp);
vec3 lightDirVector = ex_LightPos - ex_FragPos;
vec3 lightDirection = normalize(lightDirVector);
float distance = pow(length(lightDirVector), 2.0);

vec3 tex = texture(in_Texture, ex_TexCoord).rgb;

// calculate normalized normal and light direction
vec3 normalMap = texture(in_NormalMap, ex_TexCoord).rgb;
vec3 normalMapNormalized = normalize(normalMap * 2.0 - 1.0);  // this normal is in tangent space

vec3 normalsTotal = normalize(normal * normalMapNormalized);

vec4 generateAmbient() {
    vec4 ambientComponent = ex_MaterialAmbient * ex_LightAmbient * vec4(tex, 1.0);
    return ambientComponent;
}

vec4 generateDiffuse() {
    float diffuseDotProduct = dot(normalsTotal, lightDirection);
    float diffuseFactor = max(diffuseDotProduct, 0.0);
    vec4 diffuseComponent = ex_MaterialDiffuse * ex_LightDiffuse * diffuseFactor * ex_LightPower / distance;
    return diffuseComponent;
}

vec4 generateSpecular() {
    vec3 viewDirection = normalize(-ex_FragPos);
    vec3 specularSum = lightDirection + viewDirection;
    vec3 halfVector = normalize(specularSum);
    float specularDotProduct = dot(halfVector, normalsTotal);
    float specularFactor = pow(max(specularDotProduct, 0.0), ex_MaterialShininess);
    vec4 specularComponent = ex_MaterialSpecular * ex_LightSpecular * specularFactor * ex_LightPower / distance;
    return specularComponent;
}

void main(void)
{
    vec4 ambientComponent = generateAmbient();
    vec4 diffuseComponent = generateDiffuse();
    vec4 specularComponent = generateSpecular();
    vec4 component = ex_MaterialEmissive + ambientComponent + diffuseComponent + specularComponent;

    out_Color = component * ex_PaintedColor * vec4(tex, 1.0);

    float brightness = dot(out_Color.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 0.1)
        BrightColor = vec4(out_Color.rgb, 1.0);
    else
        BrightColor = vec4(1.0, 0.0, 0.0, 1.0);
}