
#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <string>

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

#include "Shader.cpp"
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

void calculoMatricesSombras() {
    float near_plane = 1.0f, far_plane = 7.5f;

    glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
}



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

std::string mouse_pos_string = "";

void funcionCallbackRatonPausa(GLFWwindow* ventana, double x, double y) {
    
    mouse_pos_string = "X: " + std::to_string(x) + ", Y: " + std::to_string(y);


}

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

    Shader normal("vertex_normal.shader", "fragment_normal.shader");
    normal.use();
    unsigned int id_programa = normal.getId();





    Shader post("vertex_post.shader", "fragment_post.shader");
    unsigned int id_post = post.getId();















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

    Cubo c4(20, { -10,-10,-25 });
    Cubo c(1);
    Cubo c2(1, { -2,0,0 });
    Cubo c3(1, { 0,-1,0 });
    

    // Preparacion -> nosotros al programar el juego
    sist_eventos->suscribir<Error>( &c2, Callbacks::resetCubo );  // Añadiendo a la lista una reaccion de Cubo cuando ocurre Error con resetCubo
    
    // Reaccion -> cuando ocurre un evento  <Error>
    //sist_eventos->publicar<Error>( new Error(0, "error de prueba") ); // Ha ocurrido un Evento Error -> avisar a todos los que reaccionan a ello


    float angulo_cubo = 0.0f;
    float angulo = 90.0f;

    




    // DEPTH TESTS
    
    Shader sombras("vertex_profundidad_luz.shader", "fragment_profundidad_luz.shader");
    unsigned int id_sombras = sombras.getId();
    Shader normal_mas_sombras("vertex_normal_con_sombras.shader", "fragment_normal_con_sombras.shader");
    unsigned int id_normal_mas_sombras = normal_mas_sombras.getId();
    Shader debug("vertex_debug.shader", "fragment_debug.shader");
    unsigned int id_debug = debug.getId();


    // 
    unsigned int FBO_sombras;
    glGenFramebuffers(1, &FBO_sombras);
    unsigned int calidad_sombras = 1024;

    unsigned int mapa_profundidad;
    glGenTextures(1, &mapa_profundidad);
    glBindTexture(GL_TEXTURE_2D, mapa_profundidad);
   
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, calidad_sombras, calidad_sombras, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


    glBindFramebuffer(GL_FRAMEBUFFER, FBO_sombras);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mapa_profundidad, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);








    // FRAME BUFFER que lee y escribe
    unsigned int FBO;
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO); //lee y escribe


    // Generar textura para el framebuffer
    unsigned int textura_fb;
    glGenTextures(1, &textura_fb);
    glBindTexture(GL_TEXTURE_2D, textura_fb);

    // Generar imagen de la textura, de momento vacía
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ANCHO_V, ALTO_V, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textura_fb, 0);

    // FRAME BUFFER que solo escribe y está optimizado para ello
    unsigned int RBO;
    glGenRenderbuffers(1, &RBO);
    glBindFramebuffer(GL_RENDERBUFFER, RBO); // equivalente a los VBOs dibujado

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, ANCHO_V, ALTO_V); // equivalente a dar el tamaño del buffer 

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO); // conectandolos entre si


    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Frame buffer completo\n";
    }
    else {
        std::cout << "Frame buffer incompleto\n";
    }

    // IMPORTANTE - DESBINDEAR
    glBindFramebuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // VAO de la pantalla - Para dibujar con el shader de fb - Paso 1
    unsigned int VAO_FB;
    glGenVertexArrays(1, &VAO_FB);
    glBindVertexArray(VAO_FB);
    unsigned int VBO_FB;
    glGenBuffers(1, &VBO_FB);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_FB);

    float vertices_fb[] = {
       // X   Y    S, T 
         -1,  1,   0, 1,      // vertice 0  
         -1, -1,   0, 0,      // vertice 1
          1, -1,   1, 0,      // vertice 2 

         -1,  1,   0, 1,      // vertice 4
          1, -1,   1, 0,      // vertice 5
          1,  1,   1, 1,      // vertice 6 
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 6, vertices_fb, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0); // pos
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))); // color
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);




    // Para usar el frambuffer nuevo hay que:
    // 1. Renderizar todo con el nuevo Framebuffer - es de lectura y escritura - quiero poder leer los datos de la escena
    // 2. Bindear al framebuffer por defecto - 0 
    // 3. Dibujar, sobre la pantalla, lo que hay en el framebuffer, modificado 
    //     3.1. Va a requerir un dibujo sobre la pantalla - shader que dibuje la pantalla (porque es un textura)
    //     3.2. Dibujar un plano semitransparente en el viewport








    
    

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

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    do {
 
        float ahora = glfwGetTime();
        //std::cout << "ahora: " << ahora << "\n";
        dif_tiempo = ahora - ultimo_tiempo;
        //std::cout << "dif_tiempo: " << dif_tiempo << "\n";
        ultimo_tiempo = ahora;





        if (glfwGetKey(ventana, GLFW_KEY_LEFT) == GLFW_PRESS) {
            loop->buscarSistema<SistemaInput>()->lanzarAccion(GLFW_KEY_LEFT); // se ha pulsado el boton LEFT
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
        // Debo cambiar la pos/up/frente camara
        glfwSetCursorPosCallback(ventana, funcionCallbackRaton); // lo que ocurre si cambia los pos del ratón






        // 1. Comprobar la profundidad - DEPTH TESTING
        sombras.use();

        float near_luz = 1.0f, far_luz = 12.5;
        glm::mat4 matriz_rayos_luz = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_luz, far_luz);
        glm::mat4 matriz_vista_luz = glm::lookAt(glm::vec3(0.5, 0.5, 1.0), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        matriz_vista_luz = matriz_rayos_luz * matriz_vista_luz;

        int vista_luz = glGetUniformLocation(id_sombras, "vista_luz");
        glUniformMatrix4fv(vista_luz, 1, GL_FALSE, glm::value_ptr(matriz_vista_luz));
        
        glViewport(0, 0, calidad_sombras, calidad_sombras);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO_sombras);
        // 1.1 Dibujar
        glClear(GL_DEPTH_BUFFER_BIT);
        // configurar shader y calculo matrices - CALCULO DE LAS SOMBRAS
        // activar shader, matrices de transformacion, uniformes sin sombras
        // draw();

        // TEXTURAS (por si afecta el alpha)
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_id_0);

        c4.draw(id_sombras);
        c.draw(id_sombras);
        c2.draw(id_sombras);
        c3.draw(id_sombras);

        
        //sist_textos->renderText("aT", 300, -100, 2);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);




        // 2. Dibujar la escena como una textura de sombras
        normal_mas_sombras.use();
        glViewport(0, 0, ANCHO_V, ALTO_V);


        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // configurar shader y calculo matrices - CALCULO DE LAS SOMBRAS
        // activar shader, matrices de transformacion, uniformes

        int modificador_de_colorLuz = glGetUniformLocation(id_normal_mas_sombras, "colorLuz");
        glUniform3f(modificador_de_colorLuz, 1.0, 1.0, 1.0);
        int modificador_de_posLuz = glGetUniformLocation(id_normal_mas_sombras, "posLuz");
        glUniform3f(modificador_de_posLuz, 0.5, 0.5, 10.0);
        int modificador_de_intensidadAmbiente = glGetUniformLocation(id_normal_mas_sombras, "intensidadAmbiente");
        glUniform1f(modificador_de_intensidadAmbiente, 0.2);
        int modificador_de_posCamara = glGetUniformLocation(id_normal_mas_sombras, "posCamara");
        glUniform3f(modificador_de_posCamara, pos_camara.x, pos_camara.y, pos_camara.z);


        int modificador_de_datosTextura = glGetUniformLocation(id_normal_mas_sombras, "datosTextura");
        glUniform1i(modificador_de_datosTextura, 0);
        int modificador_de_mapa_sombras = glGetUniformLocation(id_normal_mas_sombras, "mapa_sombras"); // NO OS OLVIDES DE LA TEXTURA DE LA SOMBRA
        glUniform1i(modificador_de_mapa_sombras, 1);

        int modificador_de_vista_luz = glGetUniformLocation(id_normal_mas_sombras, "vista_luz");
        glUniformMatrix4fv(modificador_de_vista_luz, 1, GL_FALSE, glm::value_ptr(matriz_vista_luz));


        int vista = glGetUniformLocation(id_normal_mas_sombras, "vista");
        glm::mat4 ident2 = glm::lookAt(
            pos_camara,
            pos_camara + frente_camara,
            up_camara
        );
        glUniformMatrix4fv(vista, 1, GL_FALSE, glm::value_ptr(ident2));
        int proy = glGetUniformLocation(id_normal_mas_sombras, "proy");
        glm::mat4 ident3 = glm::perspective( // conica
            glm::radians(fov),
            (float)ANCHO_V / ALTO_V,
            0.1f,
            100.0f
        );
        glUniformMatrix4fv(proy, 1, GL_FALSE, glm::value_ptr(ident3));

        // MEZCLA DE TEXTURAS
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_id_0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, mapa_profundidad);

        // RESET
        glActiveTexture(GL_TEXTURE0);

        c4.draw(id_normal_mas_sombras);
        c.draw(id_normal_mas_sombras);
        c2.draw(id_normal_mas_sombras);
        c3.draw(id_normal_mas_sombras);
        

        //sist_textos->renderText("aT", 300, -100, 2);
        glUseProgram(id_normal_mas_sombras);




        ///////////////////////////
        // SI HABILITAIS TODO ESTO PODEIS VER LA TEXTURA QUE HEMOS GENERADO CON EL FRAMEBUFFER !!
        ///////////////////////////
        //
        //debug.use();
        //int modificador_de_near_plane = glGetUniformLocation(id_debug, "near_plane");
        //glUniform1f(modificador_de_near_plane, near_luz);
        //int modificador_de_far_plane = glGetUniformLocation(id_debug, "far_plane");
        //glUniform1f(modificador_de_far_plane, far_luz);
        //int modificador_de_depthMap = glGetUniformLocation(id_debug, "depthMap");
        //glUniform1f(modificador_de_depthMap, 0);
        //
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, mapa_profundidad);
        //glBindVertexArray(VAO_FB);
        //
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        //glBindVertexArray(0);
        //
        ///////////////////////////











        // Para usar el frambuffer nuevo hay que:
        // 1. Renderizar todo con el nuevo Framebuffer - es de lectura y escritura - quiero poder leer los datos de la escena
        // 2. Bindear al framebuffer por defecto - 0 
        // 3. Dibujar, sobre la pantalla, lo que hay en el framebuffer, modificado 
        //     3.1. Va a requerir un dibujo sobre la pantalla - shader que dibuje la pantalla (porque es un textura)
        //     3.2. Dibujar un plano semitransparente en el viewport



        if (ReporteColision reporte = c2.colision(&c)) {
            float pos_proy = glm::dot(c2.getPos(), reporte.eje_penetr);
            float pos_prev_proy = glm::dot(c2.getPosPrev(), reporte.eje_penetr);
            float dist = pos_proy - pos_prev_proy;

            float dir = (dist > 0) ? 1 : -1;
            glm::vec3 mvt = reporte.eje_penetr * reporte.dist_penetr * dir;
            c2.setPos(c2.getPos() - mvt);

            c2.setFuerza(c2.getFuerza() - (glm::dot(c2.getFuerza(), reporte.eje_penetr) * reporte.eje_penetr));
            c2.setVel(c2.getVel() - (glm::dot(c2.getVel(), reporte.eje_penetr) * reporte.eje_penetr));
        }

        c.update(dif_tiempo);
        c2.update(dif_tiempo);

        //glBindTexture(GL_TEXTURE_2D, texture_id_0);
        //c.draw(id_programa);
        //c2.draw(id_programa);
        //c3.draw(id_programa);

        //sist_textos->renderText("aT", 300, -100, 2);
        //glUseProgram(id_programa);




        /// RENDERIZADO NORMAL - END

        // EN EL FRAME BUFFER DE POST SE HA GUARDADO LA ESCENA ENTERA EN SUS BUFFERS. MIENTRAS QUE NO HAGAMOS CLEAR SIGUEN AHI


        ///////////////////////////
        //// ACTIVAR PARA POST
        //// 2. Bindear al framebuffer por defecto - 0 
        //// NOTA:  LA ESCENA ENTERA SIGUE AHI
        //glBindFramebuffer(GL_FRAMEBUFFER, 0);
        //glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT); // BUFFER - PIXELES - FRAMEBUFFER

        //// PASAR ESA ESCENA AL FRAME BUFFER NORMAL - con una textura
        //// 3. Dibujar, sobre la pantalla, lo que hay en el framebuffer, modificado 
        //post.use(); // shader fb
        ///// RENDERIZADO PANTALLA 2D - START

        //// uniformes

        //// dibujar sin EBO
        //glBindVertexArray(VAO_FB);
        //glDisable(GL_DEPTH_TEST);
        //glBindTexture(GL_TEXTURE_2D, textura_fb);

        //glDrawArrays(GL_TRIANGLES, 0, 6);
        //glBindVertexArray(0);
        ///// RENDERIZADO PANTALLA 2D - END
        ///////////////////////////

       


        glfwSwapBuffers(ventana);
        glfwPollEvents();
    } while (glfwWindowShouldClose(ventana) == 0 && glfwGetKey(ventana, GLFW_KEY_ESCAPE) != GLFW_PRESS);


}

