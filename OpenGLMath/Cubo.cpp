#pragma once

#include <iostream>
#include <vector>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "glm.hpp"
#include "ext.hpp"


class Cubo {
	float lado_cubo;
    glm::vec3 pos, giro;
    bool FLAG_CAMBIO_VERTICES;
    unsigned int VAO;

public: 
	Cubo() : lado_cubo(1), FLAG_CAMBIO_VERTICES(true), pos(0,0,0), VAO(0), giro(0, 0, 0)  {};
	Cubo(float lado) : lado_cubo(lado), FLAG_CAMBIO_VERTICES(true), pos(0, 0, 0), VAO(0), giro(0, 0, 0) {};
    Cubo(float lado, glm::vec3 pos) : lado_cubo(lado), FLAG_CAMBIO_VERTICES(true), pos(pos), VAO(0), giro(0, 0, 0) {};
    Cubo(float lado, glm::vec3 pos, glm::vec3 giro) : lado_cubo(lado), FLAG_CAMBIO_VERTICES(true), pos(pos), VAO(0), giro(giro) {};

    void setPos(glm::vec3 pos) {
        this->pos = pos;
    }
    void setPos(float x, float y, float z) {
        this->pos = glm::vec3(x,y,z);
    }

    glm::vec3 getPos() {
        return pos;
    }

    void setGiro(glm::vec3 giro) {
        this->giro = giro;
    }
    void setGiro(float x, float y, float z) {
        this->giro = glm::vec3(x, y, z);
    }

    glm::vec3 getGiro() {
        return giro;
    }

	void drawInit() {
        // INIT - Paso 1
        glGenVertexArrays(1, &VAO); // atributo
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
            12, 15, 14,
            14, 13, 12,

            // CARA IZQUIERA
            8, 9, 10,
            10, 11, 8,

            // CARA INFERIOR
            16, 19, 18,
            18, 17, 16,

            // CARA SUPERIOR
            20, 21, 22,
            20, 22, 23,

            // CARA TRASERA
            5, 6, 7,
            4, 5, 7
        };
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 3 * 12, indices, GL_DYNAMIC_DRAW);

        // INIT - Paso 3
        unsigned int VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        float vertices[] = {
            // CARA TRASERA -> Z = 0
           //X              Y             Z              R     G     B       Norm_X, Norm_Y, Norm_Z      S, T 
             0,             0,            0,            1.0f, 0.0f, 0.0f,      0,      0,      -1,       1, 0,      // vertice 0  
             0,             lado_cubo,    0,            1.0f, 0.0f, 0.0f,      0,      0,      -1,       0, 0,      // vertice 1
             lado_cubo,     lado_cubo,    0,            1.0f, 0.0f, 0.0f,      0,      0,      -1,       0, 1,      // vertice 2 
             lado_cubo,     0,            0,            1.0f, 0.0f, 0.0f,      0,      0,      -1,       1, 1,      // vertice 3 

             // CARA FRONTAL -> Z = lado_cubo
           //X              Y             Z
             0,             0,            lado_cubo,    1.0f, 0.0f, 0.0f,      0,      0,      1,        0, 0,      // vertice 4
             0,             lado_cubo,    lado_cubo,    1.0f, 0.0f, 0.0f,      0,      0,      1,        0, 1,      // vertice 5
             lado_cubo,     lado_cubo,    lado_cubo,    1.0f, 0.0f, 0.0f,      0,      0,      1,        1, 1,      // vertice 6 
             lado_cubo,     0,            lado_cubo,    1.0f, 0.0f, 0.0f,      0,      0,      1,        1, 0,      // vertice 7 

             // CARA IZQUIERDA -> X = 0
             0,             0,            0,            0.0f, 0.0f, 1.0f,      -1,     0,      0,        0, 0,      // vertice 8
             0,             0,            lado_cubo,    0.0f, 0.0f, 1.0f,      -1,     0,      0,        0, 0,      // vertice 9
             0,             lado_cubo,    lado_cubo,    0.0f, 0.0f, 1.0f,      -1,     0,      0,        0, 0,      // vertice 10 
             0,             lado_cubo,    0,            0.0f, 0.0f, 1.0f,      -1,     0,      0,        0, 0,      // vertice 11 

             // CARA DERECHA -> X = lado_cubo
             lado_cubo,     0,            0,            0.0f, 0.0f, 1.0f,      1,      0,      0,        0, 0,      // vertice 12
             lado_cubo,     0,            lado_cubo,    0.0f, 0.0f, 1.0f,      1,      0,      0,        0, 0,      // vertice 13
             lado_cubo,     lado_cubo,    lado_cubo,    0.0f, 0.0f, 1.0f,      1,      0,      0,        0, 0,      // vertice 14 
             lado_cubo,     lado_cubo,    0,            0.0f, 0.0f, 1.0f,      1,      0,      0,        0, 0,      // vertice 15 

             // CARA INFERIOR -> Y = 0
             0,             0,            0,            0.0f, 1.0f, 0.0f,      0,      -1,      0,       0, 0,      // vertice 16
             0,             0,            lado_cubo,    0.0f, 1.0f, 0.0f,      0,      -1,      0,       0, 0,      // vertice 17
             lado_cubo,     0,            lado_cubo,    0.0f, 1.0f, 0.0f,      0,      -1,      0,       0, 0,      // vertice 18 
             lado_cubo,     0,            0,            0.0f, 1.0f, 0.0f,      0,      -1,      0,       0, 0,      // vertice 19 

             // CARA SUPERIOR -> Y = lado_cubo
             0,             lado_cubo,    0,            0.0f, 1.0f, 0.0f,      0,      1,       0,       0, 0,      // vertice 20
             0,             lado_cubo,    lado_cubo,    0.0f, 1.0f, 0.0f,      0,      1,       0,       0, 0,      // vertice 21
             lado_cubo,     lado_cubo,    lado_cubo,    0.0f, 1.0f, 0.0f,      0,      1,       0,       0, 0,      // vertice 22 
             lado_cubo,     lado_cubo,    0,            0.0f, 1.0f, 0.0f,      0,      1,       0,       0, 0,      // vertice 23 
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 11 * 24, vertices, GL_DYNAMIC_DRAW);


        // INIT - Paso 4
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0); // pos
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float))); // color
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float))); // normal
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(9 * sizeof(float))); // text ST
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);

        // INIT - Paso 5
        glBindVertexArray(0); // VAO
	}

	void draw(int id_programa) {
        if (FLAG_CAMBIO_VERTICES) {
            drawInit();
            FLAG_CAMBIO_VERTICES = false;
        }
        
        int modelo = glGetUniformLocation(id_programa, "modelo");
        glm::mat4 ident = glm::mat4(1.0f);

       
        
        ident = glm::translate(ident, pos);
        ident = glm::rotate(ident, glm::radians(-giro.x), glm::vec3(1.0f, 0.0f, 0.0f));
        ident = glm::rotate(ident, glm::radians(-giro.y), glm::vec3(0.0f, 1.0f, 0.0f));
        ident = glm::rotate(ident, glm::radians(-giro.z), glm::vec3(0.0f, 0.0f, 1.0f));

       

        //ident = glm::scale(ident, glm::vec3(1, 1, 1));

        glUniformMatrix4fv(modelo, 1, GL_FALSE, glm::value_ptr(ident));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 3 * 12, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

	}

    float left() {
        return this->pos.x;
        //this->pos.x + (lado_cubo) * glm::sin(glm::radians(giro.z)) * glm::sin(glm::radians(giro.y));
    }
    float right() {
        return this->pos.x + lado_cubo;
        //this->pos.x + (lado_cubo) * glm::cos(glm::radians(giro.z)) * glm::cos(glm::radians(giro.y));
    }
    float down() {
        return this->pos.y;
    }
    float up() {
        return this->pos.y + lado_cubo;
    }
    float front() {
        return this->pos.z +lado_cubo;
    }
    float back() {
        return this->pos.z;
    }


    /*bool colision(Cubo* otro) {
        std::cout << this->left() << "  " << this->right() << "\n";

        return this->left() < otro->right() && this->right() > otro->left() &&
            this->down() < otro->up() && this->up() > otro->down() && 
            this->back() < otro->front() && this->front() > otro->back();
    }*/

    bool colision(Cubo* otro) {

        std::vector<glm::vec3> vertices = verticesTransf(this);
        std::vector<glm::vec3> vertices_otro = verticesTransf(otro);

        std::vector<glm::vec3> normales = normalesTransf(this);
        std::vector<glm::vec3> normales_otro = normalesTransf(otro);

        std::vector<glm::vec3> ejes;
        for (int i = 0; i < normales.size(); i++) {
            ejes.push_back(normales[i]);
            ejes.push_back(normales_otro[i]);
        }

        //std::vector<float> vertices_proyectados;
        float min, min_otro, min_total, max, max_otro, max_total;

        for (int i = 0; i < ejes.size(); i++) {
            min = min_otro = min_total = INT_MAX;
            max = max_otro = max_total = INT_MIN;

            for (int j = 0; j < vertices.size(); j++) {
                //vertices_proyectados.push_back( glm::dot(vertices[j], ejes[i]) );
                float vert_proy = glm::dot(vertices[j], ejes[i]);
                if (min > vert_proy) min = vert_proy;
                if (max < vert_proy) max = vert_proy;
                if (min_total > vert_proy) min_total = vert_proy;
                if (max_total < vert_proy) max_total = vert_proy;

                float vert_proy_otro = glm::dot(vertices_otro[j], ejes[i]);
                if (min_otro > vert_proy_otro) min_otro = vert_proy_otro;
                if (max_otro < vert_proy_otro) max_otro = vert_proy_otro;
                if (min_total > vert_proy_otro) min_total = vert_proy_otro;
                if (max_total < vert_proy_otro) max_total = vert_proy_otro;
            }

            std::cout << "Total:" << max_total - min_total << "\n";
            std::cout << "Yo:" << max - min << "\n";
            std::cout << "otro:" << max_otro - min_otro << "\n";

            if (max_total - min_total > max - min + max_otro - min_otro) {
                return false;
            }
        }

        return true;
    }

    std::vector<glm::vec3> verticesTransf(Cubo* cubo) {
        glm::vec3 vertices[] = {
            {0,                  0,                  0},
            {cubo->lado_cubo,    0,                  0},
            {cubo->lado_cubo,    cubo->lado_cubo,    0},
            {0,                  cubo->lado_cubo,    0},

            {0,                  0,                  cubo->lado_cubo},
            {cubo->lado_cubo,    0,                  cubo->lado_cubo},
            {cubo->lado_cubo,    cubo->lado_cubo,    cubo->lado_cubo},
            {0,                  cubo->lado_cubo,    cubo->lado_cubo},
        };

        glm::mat4 transf = glm::mat4(1.0f);
        transf = glm::translate(transf, cubo->pos);
        transf = glm::rotate(transf, glm::radians(-(cubo->giro.x)), glm::vec3(1.0f, 0.0f, 0.0f));
        transf = glm::rotate(transf, glm::radians(-(cubo->giro.y)), glm::vec3(0.0f, 1.0f, 0.0f));
        transf = glm::rotate(transf, glm::radians(-(cubo->giro.z)), glm::vec3(0.0f, 0.0f, 1.0f));

        std::vector<glm::vec3> vert_trans;

        for (int i = 0; i < 8; i++) {
            vert_trans.push_back( transf * glm::vec4(vertices[i], 1.0f));
        }

        return vert_trans;
    }

    std::vector<glm::vec3> normalesTransf(Cubo* cubo) {
        glm::vec3 normales[] = {
            {1, 0, 0},
            {0, 1, 0},
            {0, 0, 1},
        };

        glm::mat4 transf = glm::mat4(1.0f);
        transf = glm::rotate(transf, glm::radians(-(cubo->giro.x)), glm::vec3(1.0f, 0.0f, 0.0f));
        transf = glm::rotate(transf, glm::radians(-(cubo->giro.y)), glm::vec3(0.0f, 1.0f, 0.0f));
        transf = glm::rotate(transf, glm::radians(-(cubo->giro.z)), glm::vec3(0.0f, 0.0f, 1.0f));

        std::vector<glm::vec3> norm_trans;

        for (int i = 0; i < 3; i++) {
            norm_trans.push_back(transf * glm::vec4(normales[i], 1.0f));
        }

        return norm_trans;
    }

};