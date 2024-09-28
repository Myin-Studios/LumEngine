#version 330 core

out vec4 FragColor;

in vec3 FragPos;  // Ricevi la posizione del vertice dal vertex shader
in vec3 Normal;   // Ricevi la normale dal vertex shader
in vec2 TexCoord; // Ricevi le coordinate texture dal vertex shader

void main()
{
    // Qui puoi calcolare il colore del fragment utilizzando le normali, le coordinate texture, ecc.
    FragColor = vec4(1.0, 0.5, 0.2, 1.0); // Un colore di esempio
}