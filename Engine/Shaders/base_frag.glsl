#version 330 core
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 col;
uniform vec3 viewPos;

void main()
{
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 norm = normalize(Normal);

    // Fresnel effect
    float cosTheta = dot(norm, viewDir);

    float fresnel = .1 + (1.0 - .1) * pow(1.0 - cosTheta, 5.0);

    vec3 finalCol = FragPos;

    FragColor = vec4(finalCol, 1.0);
}