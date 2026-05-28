#version 330 core
layout (location = 0) in vec3 posicion;
layout (location = 1) in vec2 texturaST;

out vec2 coordsTextura;

void main()
{
    coordsTextura = texturaST;
    gl_Position = vec4(posicion, 1.0);
}