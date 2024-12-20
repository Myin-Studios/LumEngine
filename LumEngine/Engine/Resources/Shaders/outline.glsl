// Resources/Shaders/outlineGeom.glsl
#version 330 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 6) out;

in VS_OUT{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
    vec3 WorldPos;
} gs_in[];

out GS_OUT{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
    vec3 WorldPos;
    bool isOutline;
} gs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float outlineScale = 0.0005;

void main() {
    // Emetti il triangolo originale
    for (int i = 0; i < 3; i++) {
        gs_out.FragPos = gs_in[i].FragPos;
        gs_out.Normal = gs_in[i].Normal;
        gs_out.TexCoord = gs_in[i].TexCoord;
        gs_out.WorldPos = gs_in[i].WorldPos;
        gs_out.isOutline = false;
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();

    // Emetti l'outline
    for (int i = 0; i < 3; i++) {
        gs_out.FragPos = gs_in[i].FragPos;
        gs_out.Normal = gs_in[i].Normal;
        gs_out.TexCoord = gs_in[i].TexCoord;
        gs_out.WorldPos = gs_in[i].WorldPos;
        gs_out.isOutline = true;

        vec4 pos = gl_in[i].gl_Position;
        vec3 normal = normalize(gs_in[i].Normal);
        pos.xyz += normal * outlineScale;
        gl_Position = pos;
        EmitVertex();
    }
    EndPrimitive();
}