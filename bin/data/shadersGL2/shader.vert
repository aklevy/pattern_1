#version 120

uniform vec2 vFollow;

void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
