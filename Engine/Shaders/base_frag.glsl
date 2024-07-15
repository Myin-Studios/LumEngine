#version 330 core
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 col;

void main()
{
    vec3 finalCol = vec3(FragPos.g) + (1.0 - vec3(FragPos.g)) * vec3(0.2, 0.2, 1.0);

    FragColor = vec4(finalCol, 1.0);
}