#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D screenTexture;

void main()
{
    FragColor = vec4(vec3(0.0, 0.0, 1.0), 1.0);
}
