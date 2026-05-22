#version 330 core

layout (location = 0) in vec3 posicion; 

uniform mat4 vista_luz; 
uniform mat4 modelo; 

void main() {
	gl_Position = vista_luz * modelo * vec4(posicion, 1.0f); 
}