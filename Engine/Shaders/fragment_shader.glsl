#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec4 FragPosLightSpace;

uniform sampler2D shadowMap;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform float roughness;
uniform vec3 objectColor;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // Esegue la divisione prospettica
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Trasforma i range da [-1,1] a [0,1]
    projCoords = projCoords * 0.5 + 0.5;
    // Ottieni il valore di profondità dello shadow map
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // Ottieni la profondità corrente
    float currentDepth = projCoords.z;
    // Controlla se siamo in ombra
    float shadow = currentDepth > closestDepth ? 1.0 : 0.0;
    return shadow;
}

void main()
{
    // Ambient
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColor;

    // Diffusion
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0);

    // Specular (optional)
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = roughness * spec * lightColor;

    // Calculating shadow...
    float shadow = ShadowCalculation(FragPosLightSpace);

    // Fresnel effect
    float cosTheta = dot(norm, viewDir);

    float fresnel = 0.1 + (1.0 - 0.1) * pow(1.0 - cosTheta, 5.0);

    vec3 col = objectColor * 1.0 - fresnel;

    vec3 result = (ambient + shadow + diffuse) * objectColor + specular;

    FragColor = vec4(result, 1.0);
}
