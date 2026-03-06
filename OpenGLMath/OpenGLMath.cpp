
#include <iostream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "glm.hpp"
#include "ext.hpp"

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

     "out vec3 posicionVertice; \n"
     "out vec3 colorVertice; \n"
     "out vec3 normalVertice; \n"

     "uniform mat4 modelo; \n"
     "uniform mat4 vista; \n"
     "uniform mat4 proy; \n"

     // Nota: tener cuidado con la matriz modelo vs normal

     "void main() {\n"
     "  gl_Position = proy * vista * modelo * vec4(posicion, 1.0f); \n"

     "  posicionVertice = posicion; \n"
     "  colorVertice = colorDatos; \n"
     "  normalVertice = normalDatos; \n"
     " }\0";

    std::string fragmentShaderCodigo =
    "#version 330 core\n"

    "out vec4 FragColor; \n"
    
    "uniform vec3 colorDesdeMain; \n"

    "in vec3 posicionVertice; \n"
    "in vec3 colorVertice; \n"
    "in vec3 normalVertice; \n"

    "uniform vec3 posLuz; \n"
    "uniform vec3 colorLuz; \n"
    "uniform float intensidadAmbiente; \n" // % de luz

    // Calc luz ambiente
    "vec3 luz_ambiente = colorLuz * intensidadAmbiente; \n"

    // Calc luz difusa
    "vec3 dir_luz = normalize(posLuz - posicionVertice); \n"
    "vec3 normalUnitaria = normalize(normalVertice); \n"
    "float intensidadDifusa = max(dot(dir_luz, normalUnitaria), 0.0f); \n"
    "vec3 luz_difusa = colorLuz * intensidadDifusa; \n"

    "vec3 luz_final = luz_ambiente + luz_difusa;"

    "void main() {\n"
    "   FragColor = vec4(colorVertice * luz_final, 1.0f); \n"
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


    // INIT - Paso 1
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // INIT - Paso 2
    unsigned int IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    unsigned int indices[] = {
        // CARA FRONTAL
        0, 3, 1,
        1, 3, 2,

        // CARA DERECHA
        3, 7, 2,
        7, 6, 2,

        // CARA IZQUIERA
        4, 0, 1,
        1, 5, 4,

        // CARA INFERIOR
        3, 4, 7,
        3, 0, 4,

        // CARA SUPERIOR
        2, 6, 5,
        2, 5, 1,

        // CARA TRASERA
        5, 6, 7,
        4, 5, 7
    };
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 3 * 12, indices, GL_DYNAMIC_DRAW);

    // INIT - Paso 3
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    float lado_cubo = 1;

    float vertices[] = {
        // CARA FRONTAL -> Z = 0
         0,             0,            0,            1.0f, 0.0f, 0.0f,       // vertice 0  
         0,             lado_cubo,    0,            1.0f, 0.0f, 0.0f,       // vertice 1
         lado_cubo,     lado_cubo,    0,            1.0f, 0.0f, 0.0f,       // vertice 2 
         lado_cubo,     0,            0,            1.0f, 0.0f, 0.0f,       // vertice 3 

         // CARA TRASERA -> Z = 0
       //X              Y             Z
         0,             0,            lado_cubo,    0.0f, 0.0f, 1.0f,       // vertice 4
         0,             lado_cubo,    lado_cubo,    0.0f, 0.0f, 1.0f,       // vertice 5
         lado_cubo,     lado_cubo,    lado_cubo,    0.0f, 0.0f, 1.0f,       // vertice 6 
         lado_cubo,     0,            lado_cubo,    0.0f, 0.0f, 1.0f,       // vertice 7 
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 8, vertices, GL_DYNAMIC_DRAW);


    // INIT - Paso 4
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // pos
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // color
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // normal
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // INIT - Paso 5
    glBindVertexArray(0); // VAO

    float tx = 0, ty = 0;
    float angulo = 90.0f;

    glm::vec3 pos_camara = glm::vec3({ 0.25, 0, 5 });
    glm::vec3 frente_camara = glm::vec3({ 0, 0, -1 });
    glm::vec3 up_camara = glm::vec3({ 0, 1, 0 });

    glEnable(GL_DEPTH_TEST);

    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, ANCHO_V, ALTO_V);

        //int modificador_de_color = glGetUniformLocation(id_programa, "colorDesdeMain");
        //float R = 0.5f;
        //glUniform3f(modificador_de_color, 0.5, 0.5, 0.0);

        // ILUMINACION

        int modificador_de_colorLuz = glGetUniformLocation(id_programa, "colorLuz");
        glUniform3f(modificador_de_colorLuz, 1.0, 1.0, 1.0);

        int modificador_de_intensidadAmbiente = glGetUniformLocation(id_programa, "intensidadAmbiente");
        glUniform1f(modificador_de_intensidadAmbiente, 0.2);


        int modelo = glGetUniformLocation(id_programa, "modelo");
        glm::mat4 ident = glm::mat4(1.0f);

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
  
        //ident = glm::translate(ident, glm::vec3(tx, ty, 0));
        //ident = glm::rotate(ident, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        //ident = glm::scale(ident, glm::vec3(1, 1, 1));

        glUniformMatrix4fv(modelo, 1, GL_FALSE, glm::value_ptr(ident));

     
        int vista = glGetUniformLocation(id_programa, "vista");
        frente_camara = glm::vec3(glm::cos(glm::radians(angulo)), 0, -glm::sin(glm::radians(angulo)));

        glm::mat4 ident2 = glm::lookAt(
            pos_camara,
            pos_camara + frente_camara,
            up_camara
        );
        glUniformMatrix4fv(vista, 1, GL_FALSE, glm::value_ptr(ident2));
        


        int proy = glGetUniformLocation(id_programa, "proy");
        glm::mat4 ident3 = glm::perspective( // conico
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


        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 3 * 12, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);


        glfwSwapBuffers(ventana);
        glfwPollEvents();
    } while (glfwWindowShouldClose(ventana) == 0 && glfwGetKey(ventana, GLFW_KEY_ESCAPE) != GLFW_PRESS);


}

