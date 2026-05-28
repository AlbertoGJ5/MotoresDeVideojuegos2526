#version 330 core


out vec4 FragColor;


in vec3 posicionVertice; 
in vec3 colorVertice; 
in vec3 normalVertice; 

in vec2 coordsTextura; 
uniform sampler2D datosTextura; 

in vec4 espacio_luz;


uniform sampler2D mapa_sombras;

uniform vec3 posLuz; 
uniform vec3 colorLuz; 
uniform float intensidadAmbiente;  // % de luz
uniform vec3 posCamara; 


float CalculoSombras(vec4 espacio_luz)
{
    // proyectar
    vec3 coords_proyectadas = espacio_luz.xyz / espacio_luz.w;

    // normalizar a [0,1]
    coords_proyectadas = coords_proyectadas * 0.5 + 0.5;

    // comprobar objecto más cercano (al que da la luz)
    float cercano = texture(mapa_sombras, coords_proyectadas.xy).r; 

    // ver si al fragmento actual le llega luz
    float profundidad = coords_proyectadas.z;

    // if-else para ver si es sombra o luz (y que no sea un %)
    float sombra = profundidad > cercano  ? 1.0 : 0.0;

    return sombra;
}

void main()
{           
    vec3 texturaFragmento = texture(datosTextura, coordsTextura).rgb;
    vec3 normalUnitaria = normalize(normalVertice);

    // luz ambiente
    vec3 luz_ambiente = colorLuz * intensidadAmbiente;

    // luz difusa
    vec3 dir_luz = normalize(posLuz - posicionVertice);
    float diff = max(dot(dir_luz, normalUnitaria), 0.0);
    vec3 luz_difusa = diff * colorLuz;

    // luz especular
    vec3 dir_camara = normalize(posCamara - posicionVertice);
    vec3 dir_reflejada = reflect(-dir_luz, normalUnitaria);
    vec3 dir_media = normalize(dir_luz + dir_camara);  // Usamos direccion media esta vez
    float fuerzaEspecular = pow(max(dot(normalUnitaria, dir_media), 0.0), 64.0);
    vec3 luz_especular = fuerzaEspecular * colorLuz;    

    // calcular sombras
    float sombra = CalculoSombras(espacio_luz);
    
    vec3 luz = (luz_ambiente + (1.0 - sombra) * (luz_difusa + luz_especular)) * texturaFragmento * colorVertice;    
    FragColor = vec4(luz, 1.0);
}