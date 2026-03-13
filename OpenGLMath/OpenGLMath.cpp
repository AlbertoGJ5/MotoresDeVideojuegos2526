
#include <iostream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "glm.hpp"
#include "ext.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Cubo.cpp"

GLFWwindow* ventana;
const unsigned int ANCHO_V = 1024, ALTO_V = 768;

//using namespace glm;

int main()
{
    glfwInit();

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    ventana = glfwCreateWindow(ANCHO_V, ALTO_V, "Motor", NULL, NULL);

    glfwMakeContextCurrent(ventana);
    glewExperimental = GL_TRUE;

    glewInit();

    //glfwSetInputMode(ventana, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    std::string vertexShaderCodigo =
     "#version 330 core\n"

     "layout (location = 0) in vec3 posicion; \n"
     "layout (location = 1) in vec3 colorDatos; \n"
     "layout (location = 2) in vec3 normalDatos; \n"
     "layout (location = 3) in vec2 texturaST; \n"

     "out vec3 posicionVertice; \n"
     "out vec3 colorVertice; \n"
     "out vec3 normalVertice; \n"

     "out vec2 coordsTextura; \n"

     "uniform mat4 modelo; \n"
     "uniform mat4 vista; \n"
     "uniform mat4 proy; \n"

     // Nota: tener cuidado con la matriz modelo vs normal

     "void main() {\n"
     "  gl_Position = proy * vista * modelo * vec4(posicion, 1.0f); \n"

     "  posicionVertice = (modelo * vec4(posicion, 1.0f)).xyz; \n"
     "  colorVertice = colorDatos; \n"
     "  normalVertice = mat3(transpose(inverse(modelo))) * normalDatos; \n"

     "  coordsTextura = texturaST; \n" 
     " }\0";

    std::string fragmentShaderCodigo =
    "#version 330 core\n"

    "out vec4 FragColor; \n"
    
    "uniform vec3 colorDesdeMain; \n"

    "in vec3 posicionVertice; \n"
    "in vec3 colorVertice; \n"
    "in vec3 normalVertice; \n"

    "in vec2 coordsTextura; \n"
    "uniform sampler2D datosTextura; \n"

    "uniform vec3 posLuz; \n"
    "uniform vec3 colorLuz; \n"
    "uniform float intensidadAmbiente; \n" // % de luz
    "uniform vec3 posCamara; \n"

    // Calc luz ambiente
    "vec3 luz_ambiente = colorLuz * intensidadAmbiente; \n"

    // Calc luz difusa
    "vec3 dir_luz = normalize(posLuz - posicionVertice); \n"
    "vec3 normalUnitaria = normalize(normalVertice); \n"
    "float intensidadDifusa = max(dot(dir_luz, normalUnitaria), 0.0f); \n"
    "vec3 luz_difusa = colorLuz * intensidadDifusa * (1.0f - intensidadAmbiente ); \n"

    // Calc luz especular
    "vec3 dir_camara = normalize(posCamara - posicionVertice); \n"
    "vec3 dir_reflejada = reflect(-dir_luz, normalVertice); \n"
    "float fuerzaEspecular = 1.0f;"
    "float intensidadEspecular = pow( max(dot(dir_camara, dir_reflejada), 0.0f), 32); \n"
    "vec3 luz_especular = colorLuz * intensidadEspecular * fuerzaEspecular; \n"

    "vec3 luz_final = luz_ambiente + luz_difusa + luz_especular;"

    "void main() {\n"
    "   FragColor = texture(datosTextura, coordsTextura) * vec4( colorVertice * luz_final, 1.0f); \n"
    " }\0";

    // Vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* temporal_vs = vertexShaderCodigo.c_str();
    glShaderSource(vertexShader, 1, &temporal_vs, NULL);
    glCompileShader(vertexShader);

    int exito;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &exito);
    if (exito) {
        std::cout << "Se ha compilado el Vertex Shader\n";
    }
    else {
        std::cout << "Ha fallado el Vertex Shader\n";
    }

    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* temporal_fs = fragmentShaderCodigo.c_str();
    glShaderSource(fragmentShader, 1, &temporal_fs, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &exito);
    if (exito) {
        std::cout << "Se ha compilado el Fragment Shader\n";
    }
    else {
        std::cout << "Ha fallado el Fragment Shader\n";
    }

    unsigned int id_programa = glCreateProgram();
    glAttachShader(id_programa, vertexShader);
    glAttachShader(id_programa, fragmentShader);
    glLinkProgram(id_programa);

    glUseProgram(id_programa);



    // TEXTURAS

    int ancho_textura, alto_textura, numero_canales;
    unsigned char* datos_textura = stbi_load("texture/brick_texture.jpg", &ancho_textura, &alto_textura, &numero_canales, 0);

    unsigned int texture_id_0;
    glGenTextures(1, &texture_id_0);
    glBindTexture(GL_TEXTURE_2D, texture_id_0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (datos_textura) {
        std::cout << "Textura cargada\n";
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ancho_textura, alto_textura, 0, GL_RGB, GL_UNSIGNED_BYTE, datos_textura);
        // target -> tipo de texture
        // nivel del mipmap -> 0
        // canales de la textura final internamente
        // dimensiones (alto y ancho)
        // borde
        // formato si se pregunta
        // tipo de datos (unsigned char es 1 byte) (unsigned int son 4 bytes)
        // datos
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "No se ha encontrado la textura\n";
    }
    // liberar los datos
    stbi_image_free(datos_textura);





    float angulo_cubo = 0.0f;
    float angulo = 90.0f;

    glm::vec3 pos_camara = glm::vec3({ 0.25, 0, 5 });
    glm::vec3 frente_camara = glm::vec3({ 0, 0, -1 });
    glm::vec3 up_camara = glm::vec3({ 0, 1, 0 });

    Cubo c(1);
    Cubo c2(1, {-2,0,0} );

    glEnable(GL_DEPTH_TEST);

    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, ANCHO_V, ALTO_V);

        // ILUMINACION

        int modificador_de_colorLuz = glGetUniformLocation(id_programa, "colorLuz");
        glUniform3f(modificador_de_colorLuz, 1.0, 1.0, 1.0);

        int modificador_de_posLuz = glGetUniformLocation(id_programa, "posLuz");
        glUniform3f(modificador_de_posLuz, 0.5, 0.5, 10.0);

        int modificador_de_intensidadAmbiente = glGetUniformLocation(id_programa, "intensidadAmbiente");
        glUniform1f(modificador_de_intensidadAmbiente, 0.2);

        int modificador_de_posCamara = glGetUniformLocation(id_programa, "posCamara");
        glUniform3f(modificador_de_posCamara, pos_camara.x, pos_camara.y, pos_camara.z);

        int modificador_de_datosTextura = glGetUniformLocation(id_programa, "datosTextura");
        glUniform1i(modificador_de_datosTextura, 0);


        if (glfwGetKey(ventana, GLFW_KEY_LEFT) == GLFW_PRESS) {
            angulo += 0.5;
        }
        else if (glfwGetKey(ventana, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            angulo -= 0.5;
        }

        if (glfwGetKey(ventana, GLFW_KEY_UP) == GLFW_PRESS) {
            pos_camara += frente_camara * 0.3f;
        }
        else if (glfwGetKey(ventana, GLFW_KEY_DOWN) == GLFW_PRESS) {
            pos_camara -= frente_camara * 0.3f;
        }

        if (glfwGetKey(ventana, GLFW_KEY_D) == GLFW_PRESS) {
            c2.setPos(c2.getPos() + glm::vec3(0.1, 0, 0));
        }
        else if (glfwGetKey(ventana, GLFW_KEY_A) == GLFW_PRESS) {
            c2.setPos(c2.getPos() + glm::vec3(-0.1, 0, 0));
        }
        else if (glfwGetKey(ventana, GLFW_KEY_W) == GLFW_PRESS) {
            c2.setPos(c2.getPos() + glm::vec3(0, 0.1, 0));
        }
        else if (glfwGetKey(ventana, GLFW_KEY_S) == GLFW_PRESS) {
            c2.setPos(c2.getPos() + glm::vec3(0, -0.1, 0));
        }
        else if (glfwGetKey(ventana, GLFW_KEY_Q) == GLFW_PRESS) {
            c2.setGiro(c2.getGiro() + glm::vec3(0, 0, 0.5));
        }
        else if (glfwGetKey(ventana, GLFW_KEY_E) == GLFW_PRESS) {
            c2.setGiro(c2.getGiro() + glm::vec3(0, 0, -0.5));
        }
  
     
        int vista = glGetUniformLocation(id_programa, "vista");
        frente_camara = glm::vec3(glm::cos(glm::radians(angulo)), 0, -glm::sin(glm::radians(angulo)));

        glm::mat4 ident2 = glm::lookAt(
            pos_camara,
            pos_camara + frente_camara,
            up_camara
        );
        glUniformMatrix4fv(vista, 1, GL_FALSE, glm::value_ptr(ident2));
        


        int proy = glGetUniformLocation(id_programa, "proy");
        glm::mat4 ident3 = glm::perspective( // conica
            glm::radians(45.0f),
            (float)ANCHO_V / ALTO_V,
            0.1f,
            100.0f
        );
        //glm::mat4 ident3 = glm::ortho( // conico
        //    -1.0f,
        //    1.0f,
        //    -1.0f,
        //    1.0f,
        //    0.1f,
        //    100.0f
        //);
        glUniformMatrix4fv(proy, 1, GL_FALSE, glm::value_ptr(ident3));

        c.draw(id_programa);
        c2.draw(id_programa);

        if (c2.colision(&c)) {
            std::cout << "COLISION\n";
        }
        else {
            std::cout << "NO COLISION\n";
        }

        glfwSwapBuffers(ventana);
        glfwPollEvents();
    } while (glfwWindowShouldClose(ventana) == 0 && glfwGetKey(ventana, GLFW_KEY_ESCAPE) != GLFW_PRESS);


}

