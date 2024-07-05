#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec4 FragPosLightSpace;

uniform float ambientStrength = 0.5;

uniform sampler2D shadowMap;
uniform vec3 lightPos;
uniform vec3 lightPos1;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform float roughness;
uniform vec3 objectColor;

float PI = 3.14159265359;

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

float DistributionGGX(vec3 N, vec3 H, float a)
{
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom    = a2;
    float denom  = (NdotH2 * (a2 - 1.0) + 1.0);
    denom        = PI * denom * denom;

    return nom / denom;
}

void main()
{
    // Ambient
    vec3 ambient = ambientStrength * lightColor;

    // Diffusion
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 lightDir1 = normalize(lightPos1 - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    float diff1 = max(dot(norm, lightDir1), 0.0);
    vec3 diffuse = (diff + diff1) * vec3(1.0);

    // Specular (optional)
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 reflectDir1 = reflect(-lightDir1, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    float spec1 = pow(max(dot(viewDir, reflectDir1), 0.0), 32);
    vec3 specular = roughness * (spec + spec1) * lightColor;

    // Calculating shadow...
    float shadow = ShadowCalculation(FragPosLightSpace);

    // Fresnel effect
    float cosTheta = dot(norm, viewDir);

    float fresnel = 2.0 + (1.0 - 2.0) * pow(1.0 - cosTheta, 5.0);

    vec3 col = objectColor * fresnel;

    float NDF_GGXTR = DistributionGGX(norm, lightDir, roughness);

    vec3 result = (ambient + diffuse) * objectColor + specular;

    FragColor = vec4(result, 1.0);
}
