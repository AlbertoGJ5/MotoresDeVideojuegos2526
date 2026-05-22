#version 330 core

out vec4 FragColor; 

in vec2 coordsTextura; 
uniform sampler2D datosTextura; 

const float offset = 1.0 / 300.0;

void main() {

   vec2 offset_matrix[9] = vec2[](
       vec2(-offset,  offset),
       vec2(      0,  offset),
       vec2( offset,  offset),

       vec2(-offset,       0),
       vec2(      0,       0),
       vec2( offset,       0),

       vec2(-offset, -offset),
       vec2(      0, -offset),
       vec2( offset, -offset) 
   );


   vec3 pixeles_adyacentes[9];
   for (int i = 0; i < 9; i++) {
       pixeles_adyacentes[i] = vec3( texture(datosTextura, coordsTextura + offset_matrix[i]) );
   };

/*     float kernel[9] = float[] (
       -1, -1, -1,
       -1,  9, -1,
       -1, -1, -1 
   );*/

   float kernel[9] = float[] (
        1.0/16,  2.0/16,  1.0/16,
        2.0/16,  4.0/16,  2.0/16,
        1.0/16,  2.0/16,  1.0/16 
   ); 

   vec3 color_tras_kernel;
   for (int i = 0; i < 9; i++) {
       color_tras_kernel += kernel[i] * pixeles_adyacentes[i];
   };

   FragColor = vec4(color_tras_kernel, 1.0);
        
        

        
// escala de grises
//   FragColor = texture(datosTextura, coordsTextura); 
//   float media = FragColor.r * 0.2126 + FragColor.g * 0.7152 + FragColor.b * 0.0722; 
//   FragColor = vec4(media, media, media, 1); 
};