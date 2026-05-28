#version 330 core

layout (location = 0) in vec3 posicion; 
layout (location = 1) in vec3 colorDatos; 
layout (location = 2) in vec3 normalDatos; 
layout (location = 3) in vec2 texturaST; 

out vec3 posicionVertice; 
out vec3 colorVertice; 
out vec3 normalVertice; 

out vec2 coordsTextura; 
out vec4 espacio_luz;

uniform mat4 modelo; 
uniform mat4 vista; 
uniform mat4 proy; 

uniform mat4 vista_luz; 

void main() {
	gl_Position = proy * vista * modelo * vec4(posicion, 1.0f); 

	posicionVertice = (modelo * vec4(posicion, 1.0f)).xyz; 
	colorVertice = colorDatos; 
	normalVertice = mat3(transpose(inverse(modelo))) * normalDatos; 

	coordsTextura = texturaST;  

	espacio_luz = vista_luz * vec4(posicionVertice, 1.0);
}