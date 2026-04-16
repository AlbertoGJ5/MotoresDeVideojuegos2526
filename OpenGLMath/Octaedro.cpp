
#pragma once

#include <iostream>
#include <vector>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "glm.hpp"
#include "ext.hpp"

#include "Cubo.cpp"

class Octaedro {
    float ancho, alto, masa;
    glm::vec3 pos, vel, fuerza;
    bool FLAG_CAMBIO_VERTICES;
    unsigned int VAO;

public:
    Octaedro() : ancho(1), alto(1), FLAG_CAMBIO_VERTICES(true), pos(0, 0, 0), VAO(0), vel(0, 0, 0), fuerza(0, 0, 0), masa(1) {};
    Octaedro(float ancho, float alto) : ancho(ancho), alto(alto), FLAG_CAMBIO_VERTICES(true), pos(0, 0, 0), VAO(0), vel(0, 0, 0), fuerza(0, 0, 0), masa(1) {};
    Octaedro(float ancho, float alto, glm::vec3 pos) : ancho(ancho), alto(alto), FLAG_CAMBIO_VERTICES(true), pos(glm::vec3(pos)), VAO(0), vel(0, 0, 0), fuerza(0, 0, 0), masa(1) {};

    void setPos(glm::vec3 pos) {
        this->pos = pos;
    }
    void setPos(float x, float y, float z) {
        this->pos = glm::vec3(x, y, z);
    }

    glm::vec3 getPos() {
        return pos;
    }

    void setVel(glm::vec3 vel) {
        this->vel = vel;
    }
    void setVel(float x, float y, float z) {
        this->vel = glm::vec3(x, y, z);
    }
    glm::vec3 getVel() {
        return vel;
    }

    void setAcel(glm::vec3 acel) {
        this->fuerza = acel * masa;
    }
    void setAcel(float x, float y, float z) {
        this->fuerza = glm::vec3(x, y, z) * masa;
    }
    glm::vec3 getAcel() {
        return fuerza / masa;
    }

    void setFuerza(glm::vec3 fuerza) {
        this->fuerza = fuerza;
    }
    void setFuerza(float x, float y, float z) {
        this->fuerza = glm::vec3(x, y, z);
    }
    glm::vec3 getFuerza() {
        return fuerza;
    }

    void drawInit() {
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        unsigned int IBO;
        glGenBuffers(1, &IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

        unsigned int indices[] = {
            0, 1, 2,
            3, 4, 5,

            6, 7, 8,
            9, 10, 11,

            12, 13, 14,
            15, 16, 17,

            18, 19, 20,
            21, 22, 23,
        };
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 3 * 12, indices, GL_DYNAMIC_DRAW);

        unsigned int VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        float vertices[] = {
             0,             alto / 2,     0,            1.0f, 1.0f, 1.0f,      alto / 2,    ancho / 2,     0,          0,0, // vertice 0  
             ancho / 2,     0,            ancho / 2,    1.0f, 1.0f, 1.0f,      alto / 2,    ancho / 2,     0,          0,0, // vertice 1
             ancho / 2,     0,            -ancho / 2,   1.0f, 1.0f, 1.0f,      alto / 2,    ancho / 2,     0,          0,0, // vertice 2 

             0,             alto / 2,     0,            1.0f, 1.0f, 1.0f,      0,           ancho / 2,     -alto / 2,  0,0, // vertice 3 
             ancho / 2,     0,            -ancho / 2,   1.0f, 1.0f, 1.0f,      0,           ancho / 2,     -alto / 2,  0,0, // vertice 4
             -ancho / 2,    0,            -ancho / 2,   1.0f, 1.0f, 1.0f,      0,           ancho / 2,     -alto / 2,  0,0, // vertice 5

             0,             alto / 2,     0,            1.0f, 1.0f, 1.0f,      -alto / 2,   ancho / 2,     0,          0,0, // vertice 6 
             -ancho / 2,    0,            -ancho / 2,   1.0f, 1.0f, 1.0f,      -alto / 2,   ancho / 2,     0,          0,0, // vertice 7 
             -ancho / 2,    0,            ancho / 2,    1.0f, 1.0f, 1.0f,      -alto / 2,   ancho / 2,     0,          0,0, // vertice 8

             0,             alto / 2,     0,            1.0f, 1.0f, 1.0f,      0,           ancho / 2,     alto / 2,   0,0, // vertice 9
             -ancho / 2,    0,            ancho / 2,    1.0f, 1.0f, 1.0f,      0,           ancho / 2,     alto / 2,   0,0, // vertice 10 
             ancho / 2,     0,            ancho / 2,    1.0f, 1.0f, 1.0f,      0,           ancho / 2,     alto / 2,   0,0, // vertice 11 

             0,             -alto / 2,    0,            1.0f, 1.0f, 1.0f,      alto / 2,    -ancho / 2,     0,         0,0, // vertice 12
             ancho / 2,     0,            -ancho / 2,   1.0f, 1.0f, 1.0f,      alto / 2,    -ancho / 2,     0,         0,0, // vertice 13
             ancho / 2,     0,            ancho / 2,    1.0f, 1.0f, 1.0f,      alto / 2,    -ancho / 2,     0,         0,0, // vertice 14 

             0,             -alto / 2,    0,            1.0f, 1.0f, 1.0f,      0,           -ancho / 2,     -alto / 2, 0,0, // vertice 15 
             -ancho / 2,    0,            -ancho / 2,   1.0f, 1.0f, 1.0f,      0,           -ancho / 2,     -alto / 2, 0,0, // vertice 16
             ancho / 2,     0,            -ancho / 2,   1.0f, 1.0f, 1.0f,      0,           -ancho / 2,     -alto / 2, 0,0, // vertice 17

             0,             -alto / 2,    0,            1.0f, 1.0f, 1.0f,      -alto / 2,   -ancho / 2,     0,         0,0, // vertice 18 
             -ancho / 2,    0,            ancho / 2,    1.0f, 1.0f, 1.0f,      -alto / 2,   -ancho / 2,     0,         0,0, // vertice 19 
             -ancho / 2,    0,            -ancho / 2,   1.0f, 1.0f, 1.0f,      -alto / 2,   -ancho / 2,     0,         0,0, // vertice 20

             0,             -alto / 2,    0,            1.0f, 1.0f, 1.0f,      0,           -ancho / 2,     alto / 2,  0,0, // vertice 21
             ancho / 2,     0,            ancho / 2,    1.0f, 1.0f, 1.0f,      0,           -ancho / 2,     alto / 2,  0,0, // vertice 22 
             ancho / 2,     0,            -ancho / 2,   1.0f, 1.0f, 1.0f,      0,           -ancho / 2,     alto / 2,  0,0, // vertice 23 
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 11 * 24, vertices, GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0); // pos
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float))); // color
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float))); // normal
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(9 * sizeof(float))); // text ST
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);

        glBindVertexArray(0);
    }

    void draw(int id_programa) {
        if (FLAG_CAMBIO_VERTICES) {
            drawInit();
            FLAG_CAMBIO_VERTICES = false;
        }

        int modelo = glGetUniformLocation(id_programa, "modelo");
        glm::mat4 ident = glm::mat4(1.0f);

        ident = glm::translate(ident, pos);


        glUniformMatrix4fv(modelo, 1, GL_FALSE, glm::value_ptr(ident));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 3 * 8, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void update(float delta_time) {

        vel += fuerza / masa * delta_time;
        pos += vel * delta_time;

        fuerza -= vel * 0.5f;
        if (vel.x > 1) {
            fuerza.x = 0;
            vel.x = 1;
        }
        else if (vel.x < -1) {
            fuerza.x = 0;
            vel.x = -1;
        }

    }

    /*
    ReporteColision colision(Octaedro* otro) {
        Cubo* c = new Cubo(alto, pos);
        return otro->colision(c);
    }*/

    float left() {
        return this->pos.x - ancho / 2;
    }
    float right() {
        return this->pos.x + ancho / 2;
    }
    float down() {
        return this->pos.y - alto / 2;
    }
    float up() {
        return this->pos.y + alto / 2;
    }
    float front() {
        return this->pos.z + ancho / 2;
    }
    float back() {
        return this->pos.z - ancho / 2;
    }

    ReporteColision colision(Octaedro* otro) {
        std::cout << this->left() << "  " << this->right() << "\n";

        bool col = this->left() < otro->right() && this->right() > otro->left() &&
            this->down() < otro->up() && this->up() > otro->down() &&
            this->back() < otro->front() && this->front() > otro->back();

        return ReporteColision(col);
    }

    ReporteColision colision(Cubo* otro) {
        float max = alto > ancho ? alto : ancho;
        Cubo* c = new Cubo(max, pos);
        return otro->colision(c);
    }

};