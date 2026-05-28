#version 330 core

out vec4 FragColor; 
    
uniform vec3 colorDesdeMain; 

in vec3 posicionVertice; 
in vec3 colorVertice; 
in vec3 normalVertice; 

in vec2 coordsTextura; 
uniform sampler2D datosTextura; 

uniform vec3 posLuz; 
uniform vec3 colorLuz; 
uniform float intensidadAmbiente;  // % de luz
uniform vec3 posCamara; 

// Calc luz ambiente
vec3 luz_ambiente = colorLuz * intensidadAmbiente; 

// Calc luz difusa
vec3 dir_luz = normalize(posLuz - posicionVertice); 
vec3 normalUnitaria = normalize(normalVertice); 
float intensidadDifusa = max(dot(dir_luz, normalUnitaria), 0.0f); 
vec3 luz_difusa = colorLuz * intensidadDifusa * (1.0f - intensidadAmbiente ); 

// Calc luz especular
vec3 dir_camara = normalize(posCamara - posicionVertice); 
vec3 dir_reflejada = reflect(-dir_luz, normalUnitaria); 
float fuerzaEspecular = 1.0f;
float intensidadEspecular = pow( max(dot(dir_camara, dir_reflejada), 0.0f), 32); 
vec3 luz_especular = colorLuz * intensidadEspecular * fuerzaEspecular; 

vec3 luz_final = luz_ambiente + luz_difusa + luz_especular;

void main() {
    FragColor = texture(datosTextura, coordsTextura) * vec4( colorVertice * luz_final, 1.0f); 
}