#version 330 core

out vec4 FragColor;
in vec3 WorldPos;  // Posizione del frammento nel sistema di coordinate della skysphere (centrata sull'osservatore)

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

void main() {

    vec3 topColor = vec3(0.5, 0.7, 1.0);
    vec3 downColor = vec3(0.1, 0.3, 0.6);
    vec3 finalSkyColor;

    float smoothingFactor = clamp(Normal.y, -1.0, 1.0);

    if (smoothingFactor < 0.0) {
        finalSkyColor = mix(vec3(1.0), topColor, 0.0 - smoothingFactor);
    } else {
        finalSkyColor = mix(downColor, vec3(1.0), 1.0 - smoothingFactor);
    }

    FragColor = vec4(finalSkyColor, 1.0);

}