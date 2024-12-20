#version 330 core

layout(location = 0) in vec3 aPos; // Posizione del vertice
layout(location = 1) in vec3 aNormal; // Normale del vertice (importante per l'espansione del bordo)

uniform mat4 model; // Matrice del modello
uniform mat4 view; // Matrice della vista
uniform mat4 projection; // Matrice di proiezione
uniform float outlineThickness = 0.05; // Spessore del contorno

out vec3 fragPos; // Posizione del vertice da passare al fragment shader

void main()
{
    // Calcola la posizione del vertice espandendolo lungo la normale
    vec3 expandedPosition = aPos + normalize(aNormal) * outlineThickness;

    // Trasformazione finale della posizione del vertice
    gl_Position = projection * view * model * vec4(expandedPosition, 1.0);

    fragPos = expandedPosition; // Passa la posizione espansa al fragment shader
}
