#version 330 core
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 col;

void main()
{
    FragColor = vec4(vec3(1.0, 0.0, 0.0), 1.0);
}