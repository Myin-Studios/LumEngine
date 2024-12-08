#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D depthTexture;

void main()
{
    float depth = texture(depthTexture, TexCoords).r;

    // Amplifica i valori di profondità per debug
    depth = depth * 10.0; // Modifica il moltiplicatore secondo necessità

    FragColor = vec4(texture(depthTexture, TexCoords).rgb, 1.0);
}