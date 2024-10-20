#version 330 core // Specifica la versione del GLSL

layout(location = 0) in vec3 aPos;    // Posizione del vertice (attributo 0)
layout(location = 1) in vec3 aNormal; // Normale del vertice (attributo 1)
layout(location = 2) in vec2 aTexCoord; // Coordinate texture (attributo 2)

out vec3 FragPos; // Posizione del vertice da passare al fragment shader
out vec3 Normal;  // Normale da passare al fragment shader
out vec2 TexCoord; // Coordinate texture da passare al fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0)); // Trasforma la posizione del vertice
    Normal = aNormal; // Passa la normale
    TexCoord = aTexCoord; // Passa le coordinate texture

    gl_Position = projection * view * vec4(FragPos, 1.0); // Calcola la posizione finale
}