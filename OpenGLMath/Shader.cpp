
#pragma once 

#include "iostream"
#include <fstream>
#include <sstream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "glm.hpp"
#include "ext.hpp"



class Shader {
    unsigned int id;

public:

    static std::string readFile(const std::string& filePath) {
        std::ifstream file(filePath);
        std::ostringstream content;
        content << file.rdbuf();
        return content.str();
    }

    unsigned int compileShader(std::string file_vs, std::string file_fs) {

        // Vertex shader
        int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        std::string vertexCode = readFile(file_vs);
        const char* temporal_vs = vertexCode.c_str();
        glShaderSource(vertexShader, 1, &temporal_vs, NULL);
        glCompileShader(vertexShader);

        int exito_v;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &exito_v);
        if (exito_v) {
            std::cout << "Se ha compilado " << file_vs << "\n";
        }
        else {
            std::cout << "Ha fallado " << file_vs << "\n";
        }

        // fragment shader
        int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        std::string fragmentCode = readFile(file_fs);
        const char* temporal_fs = fragmentCode.c_str();
        glShaderSource(fragmentShader, 1, &temporal_fs, NULL);
        glCompileShader(fragmentShader);

        int exito_f;
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &exito_f);
        if (exito_f) {
            std::cout << "Se ha compilado " << file_fs << "\n";
        }
        else {
            std::cout << "Ha fallado " << file_fs << "\n";
        }

        if (exito_v && exito_f) {
            id = glCreateProgram();
            glAttachShader(id, vertexShader);
            glAttachShader(id, fragmentShader);
            glLinkProgram(id);

            return id;
        }
        id = 0;
        return 0;
    }

public:

    Shader(std::string file_vs, std::string file_fs) {
        this->id = compileShader(file_vs, file_fs);
    }

    void use() {
        if (id) {
            glUseProgram(id);
        }
    }

    unsigned int getId() {
        return id;
    }

};