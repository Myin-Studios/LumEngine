#version 330 core
void main()
{
    // Scrivi solo la profondità
    gl_FragDepth = gl_FragCoord.z;
}
