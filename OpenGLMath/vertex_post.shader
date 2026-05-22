#version 330 core

layout (location = 0) in vec2 posicion; 
layout (location = 1) in vec2 texturaST; 

out vec2 coordsTextura; 

void main() {
  gl_Position = vec4(posicion.x, posicion.y, 0.0f, 1.0f); 
  coordsTextura = texturaST; 
};