
#pragma once

#include <iostream>
#include <vector>
#include <map>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "glm.hpp"
#include "ext.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#include "ECS.hpp"
#include "Event.cpp"
#include "Input.cpp"
#include "SistemaTexto.cpp"

#include "Cubo.cpp"
#include "Octaedro.cpp"



class Callbacks {
public:
    static void resetCubo(GameObject* obj, Evento* evento) {
        //GameObject* gm = dynamic_cast<GameObject*>(new Cubo()); // upcasting con dynamic_cast - de hijo a padre
        //Cubo* gm = static_cast<Cubo*>(new GameObject()); // downcasting con static_cast - inseguro - de padre a hijo
        //Cubo* c = dynamic_cast<Cubo*>( new GameObject() // downcasting con dynamic_cast - seguro pero GameObject tiene que tener
        //                                                   >1 funciones virtuales - de padre a hijo

        Cubo* c = dynamic_cast<Cubo*>( obj );
        if (c) {
            c->reset();
        }
        else {
            std::cout << "Conversion incorrecta\n";
        }
        
    }


    static void botonPulsado(GameObject* obj, Evento* evento) {

        //Cubo* c = dynamic_cast<Cubo*>(obj);
        EventoInput* evento_boton = dynamic_cast<EventoInput*>(evento);
        std::string accion = evento_boton->accion;

        if (evento_boton) {
            if (accion == "saltar") {

            }
            else {

            }
        }
        else {
            std::cout << "Conversion incorrecta\n";
        }

    }
};


GLFWwindow* ventana;
const unsigned int ANCHO_V = 1920, ALTO_V = 1080;

float xi = ANCHO_V / 2;
float yi = ALTO_V / 2;
float yaw = -90.f, pitch = 0;

float fov = 45.0f;

bool primer_mov_camara = true;

glm::vec3 pos_camara = glm::vec3({ 0.25, 0, 5 });
glm::vec3 frente_camara = glm::vec3({ 0, 0, -1 });
glm::vec3 up_camara = glm::vec3({ 0, 1, 0 });

void funcionCallbackRaton(GLFWwindow* ventana, double x, double y) {

    float xf = static_cast<float>(x);
    float yf = static_cast<float>(y);

    if (primer_mov_camara) {
        xi = xf;
        yi = yf;
        primer_mov_camara = false;
    }

    float sensibilidad_raton = 0.05f;

    float cambio_x = (xf - xi) * sensibilidad_raton;
    float cambio_y = (yi - yf) * sensibilidad_raton;
    xi = xf;
    yi = yf;

    // Evitar vueltas sobre si mismo
    if (pitch > 89.f) {
        pitch = 89.f;
    }
    if (pitch < -89.f) {
        pitch = -89.f;
    }

    yaw += cambio_x;
    pitch += cambio_y;

    glm::vec3 direccion_mirada;
    direccion_mirada.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direccion_mirada.y = sin(glm::radians(pitch));
    direccion_mirada.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    frente_camara = glm::normalize(direccion_mirada);
    glm::vec3 perp_plano_roll = glm::normalize(glm::cross(frente_camara, glm::vec3(0, 1, 0)));
    up_camara = glm::normalize(glm::cross(perp_plano_roll ,frente_camara));
}


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

    glfwSetInputMode(ventana, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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



    GameLoop* loop = GameLoop::getInstance();

    //SistemaEventos* sist_eventos = loop->crearSistema<SistemaEventos>();
    SistemaEventos* sist_eventos = new SistemaEventos();
    loop->addSistema(sist_eventos);
    SistemaInput* sist_inputs = new SistemaInput();
    loop->addSistema(sist_inputs);
    SistemaTexto* sist_textos = new SistemaTexto();
    sist_textos->inicializar();
    loop->addSistema(sist_textos);

    Cubo c(1);
    Cubo c2(1, { -2,0,0 });
    Cubo c3(1, { 0,-1,0 });

    // Preparacion -> nosotros al programar el juego
    sist_eventos->suscribir<Error>( &c2, Callbacks::resetCubo );  // Añadiendo a la lista una reaccion de Cubo cuando ocurre Error con resetCubo
    
    // Reaccion -> cuando ocurre un evento  <Error>
    //sist_eventos->publicar<Error>( new Error(0, "error de prueba") ); // Ha ocurrido un Evento Error -> avisar a todos los que reaccionan a ello


    float angulo_cubo = 0.0f;
    float angulo = 90.0f;

    

    

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float dif_tiempo = 0.0f;
    float ultimo_tiempo = 0.0f;

    
    Entidad e(0);
    //e.crearComponente<Cubo>();



    std::map<uint32_t, Entidad*> entidades;
    std::vector<Componente*> componentes;
    for (auto it : entidades) {
        //Componente* posible_componente = dynamic_cast<Componente*>(it.second->buscarComponente<Componente>() );
        //if (posible_componente) componentes.push_back(posible_componente);
    }

    bool pause = false;

    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, ANCHO_V, ALTO_V);
        

        if (!pause) {
            float ahora = glfwGetTime();
            //std::cout << "ahora: " << ahora << "\n";
            dif_tiempo = ahora - ultimo_tiempo;
            //std::cout << "dif_tiempo: " << dif_tiempo << "\n";
            ultimo_tiempo = ahora;

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
                //loop->buscarSistema<SistemaInput>()->lanzarAccion(GLFW_KEY_LEFT); // se ha pulsado el boton LEFT
                pos_camara -= glm::normalize(glm::cross(frente_camara, up_camara)) * 0.3f;
            }
            else if (glfwGetKey(ventana, GLFW_KEY_RIGHT) == GLFW_PRESS) {
                pos_camara += glm::normalize(glm::cross(frente_camara, up_camara)) * 0.3f;
            }



            if (glfwGetKey(ventana, GLFW_KEY_UP) == GLFW_PRESS) {
                pos_camara += frente_camara * 0.3f;
            }
            else if (glfwGetKey(ventana, GLFW_KEY_DOWN) == GLFW_PRESS) {
                pos_camara -= frente_camara * 0.3f;
            }

            if (glfwGetKey(ventana, GLFW_KEY_D) == GLFW_PRESS) {
                c2.setAcel(glm::vec3(2, 0, 0));
            }
            else if (glfwGetKey(ventana, GLFW_KEY_A) == GLFW_PRESS) {
                c2.setAcel(glm::vec3(-2, 0, 0));
            }
            else if (glfwGetKey(ventana, GLFW_KEY_W) == GLFW_PRESS) {
                c2.setPos(c2.getPos() + glm::vec3(0, 0.1, 0));
            }
            else if (glfwGetKey(ventana, GLFW_KEY_S) == GLFW_PRESS) {
                c2.setPos(c2.getPos() + glm::vec3(0, -0.1, 0));
            }
            else if (glfwGetKey(ventana, GLFW_KEY_D) == GLFW_RELEASE || glfwGetKey(ventana, GLFW_KEY_A) == GLFW_RELEASE) {
                c2.setAcel(glm::vec3(0, c2.getAcel().y, c2.getAcel().z));
                c2.setVel(glm::vec3(0, c2.getVel().y, c2.getVel().z));
            }


            if (glfwGetMouseButton(ventana, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
                //c2.setGiro(c2.getGiro() + glm::vec3(0, 0, 0.5));
                fov = 30.0f;
            }
            else if (glfwGetKey(ventana, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
                //c2.setGiro(c2.getGiro() + glm::vec3(0, 0, -0.5));
                fov = 45.0f;
            }


            int vista = glGetUniformLocation(id_programa, "vista");
            //frente_camara = glm::vec3(glm::cos(glm::radians(angulo)), 0, -glm::sin(glm::radians(angulo)));


            // Debo cambiar la pos/up/frente camara
            glfwSetCursorPosCallback(ventana, funcionCallbackRaton); // lo que ocurre si cambia los pos del ratón


            glm::mat4 ident2 = glm::lookAt(
                pos_camara,
                pos_camara + frente_camara,
                up_camara
            );
            glUniformMatrix4fv(vista, 1, GL_FALSE, glm::value_ptr(ident2));



            int proy = glGetUniformLocation(id_programa, "proy");
            glm::mat4 ident3 = glm::perspective( // conica
                glm::radians(fov),
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


            if (ReporteColision reporte = c2.colision(&c)) {
                std::cout << "COLISION\n";

                float pos_proy = glm::dot(c2.getPos(), reporte.eje_penetr);
                float pos_prev_proy = glm::dot(c2.getPosPrev(), reporte.eje_penetr);
                float dist = pos_proy - pos_prev_proy;
                std::cout << "dir: " << dist << "\n";

                float dir = (dist > 0) ? 1 : -1;
                glm::vec3 mvt = reporte.eje_penetr * reporte.dist_penetr * dir;
                std::cout << "eje:" << mvt.x << ", " << mvt.y << ", " << mvt.z << "\n";
                c2.setPos(c2.getPos() - mvt);

                //std::cout << "FUERZA:" << c2.getFuerza().x << ", " << c2.getFuerza().y << ", " << c2.getFuerza().z << "\n";

                c2.setFuerza(c2.getFuerza() - (glm::dot(c2.getFuerza(), reporte.eje_penetr) * reporte.eje_penetr));
                c2.setVel(c2.getVel() - (glm::dot(c2.getVel(), reporte.eje_penetr) * reporte.eje_penetr));


                //std::cout << "EJE:" << reporte.eje_penetr.x << ", " << reporte.eje_penetr.y << ", " << reporte.eje_penetr.z << "\n";
            }
            else {
                //std::cout << "NO COLISION\n";
            }

            c.update(dif_tiempo);
            c2.update(dif_tiempo);


            glBindTexture(GL_TEXTURE_2D, texture_id_0);
            c.draw(id_programa);
            c2.draw(id_programa);


            //glBindTexture(GL_TEXTURE_2D, glifos['v'].texture_id);
            c3.draw(id_programa);
            glBindTexture(GL_TEXTURE_2D, 0);


            // DEBERIA DETECTAR EL BOTON DONDE LOS BOTONES, CREAR UN EVENTO, Y RESOLVERLO AQUI
            if (glfwGetKey(ventana, GLFW_KEY_ENTER) == GLFW_PRESS) {
                sist_textos->renderText("TEST de texto #|@", 300, -100, 2);
                glUseProgram(id_programa);

                glfwSetInputMode(ventana, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
                pause = true;
            }
        }
        
        else {

            sist_textos->renderText("TEST de texto #|@", 300, -100, 2);
            glUseProgram(id_programa);


            if (glfwGetKey(ventana, GLFW_KEY_ENTER) == GLFW_RELEASE) {
                glfwSetInputMode(ventana, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                pause = false;
            }
        }

        glfwSwapBuffers(ventana);
        glfwPollEvents();
    } while (glfwWindowShouldClose(ventana) == 0 && glfwGetKey(ventana, GLFW_KEY_ESCAPE) != GLFW_PRESS);


}

