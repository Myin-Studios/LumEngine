#version 450 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec4 FracColor;
in vec3 ViewDir;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

uniform float ambientStrength;
uniform float specularStrength;
uniform float shininess;
uniform float scatteringFactor;

void main()
{
    vec3 toLightVector = lightPos - FragPos;
    vec3 lightDir = normalize(toLightVector);
    float distance = dot(toLightVector, toLightVector);
    float ndotl = max(0.0, dot(Normal, lightDir));
    float inversendotl = step(0.0, dot(Normal, -lightDir));
    vec3 lightCol = lightColor.rgb * ndotl / distance * 1.0;
    vec3 translucencyCol = objectColor * 2.0;
    vec3 subsurfaceCol = translucencyCol * inversendotl / distance * 1.0;
    vec3 result = subsurfaceCol * lightColor;

    //diffuse
    float diff = max(dot(Normal, lightDir), 0.0);

    //specular
    vec3 reflectDir = reflect(-lightDir, Normal);
    vec3 viewDir = normalize(ViewDir);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

    //scattering calculation
    float scattering = scatteringFactor * (1.0 - diff);

    //light attenuation

    float attenuation = 1.0 / (distance * distance);

    vec3 ambient = ambientStrength * objectColor;
    vec3 diffuse = diff * lightColor * objectColor;
    vec3 specular = specularStrength * spec * lightColor;
    vec3 scatteringColor = scattering * lightColor;

    vec3 res = ambient + (diffuse + specular + scatteringColor) * attenuation;
    float edgeTransparency = 1.0 - diff;

    FragColor = vec4(result, 1.0);
}