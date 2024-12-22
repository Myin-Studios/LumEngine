#version 330 core

layout(location = 0) in vec3 aPos; // Posizione del vertice
layout(location = 1) in vec3 aNormal; // Normale del vertice (importante per l'espansione del bordo)

uniform mat4 model; // Matrice del modello
uniform mat4 view; // Matrice della vista
uniform mat4 projection; // Matrice di proiezione
uniform float outlineThickness = 0.01; // Spessore del contorno
uniform vec3 camPos;

out vec3 fragPos; // Posizione del vertice da passare al fragment shader

void main()
{
    // Calcola la posizione del vertice nel world space
    vec3 worldPos = vec3(model * vec4(aPos, 1.0));
    
    // Calcola la distanza dalla camera
    float distanceToCamera = length(camPos - worldPos);
    
    // Scala lo spessore in base alla distanza
    float scaledThickness = outlineThickness * distanceToCamera;
    
    // Trasforma la normale in world space (assicurati che sia normalizzata)
    vec3 worldNormal = normalize(mat3(model) * aNormal);
    
    // Espandi la posizione lungo la normale scalata
    vec3 expandedPosition = worldPos + worldNormal * scaledThickness;
    
    // Trasformazione finale
    gl_Position = projection * view * vec4(expandedPosition, 1.0);
    fragPos = expandedPosition;
}
