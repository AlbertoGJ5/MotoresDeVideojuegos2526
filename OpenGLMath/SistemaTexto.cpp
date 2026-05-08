
#pragma once

#include "ECS.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H  



class SistemaTexto : public Sistema {
protected:
	struct Glifo {
		unsigned int texture_id;
		glm::vec2 size; // anchura y altura
		glm::vec2 bearing;
		unsigned int advance; // distancia al siguiente glifo
	};

    std::map<char, Glifo> glifos;
    unsigned int shader_texto = 0;
    unsigned int VAO = 0, VBO = 0;


public:
	SistemaTexto() {};
	virtual ~SistemaTexto() = default;

	void inicializar() override {
		FT_Library libreria;
		if (FT_Init_FreeType(&libreria)) {
			std::cout << "No se ha podido inicializar FreeType\n";
			return;
		}

		FT_Face font_face; // leer el archivo - guardar TODAS las letras
		if (FT_New_Face(libreria, "font/arial.ttf", 0, &font_face)) {
			std::cout << "No se ha podido leer la fuente\n";
			return;
		}

		FT_Set_Pixel_Sizes(font_face, 0, 48);

        // guarda un bitmap de pixeles enteros (i)
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // deshabilitar el alineamiento

        for (unsigned char c = 0; c <= 126; c++) {
            if (!FT_Load_Char(font_face, c, FT_LOAD_RENDER)) { // cargo el bitmap
                std::cout << "Se ha renderizado el glifo\n";

                unsigned int texture_texto;
                glGenTextures(1, &texture_texto);
                glBindTexture(GL_TEXTURE_2D, texture_texto);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // no quiero repeticiones, sino expansión
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_RED,
                    font_face->glyph->bitmap.width,
                    font_face->glyph->bitmap.rows,
                    0,
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    font_face->glyph->bitmap.buffer
                );

                Glifo glifo_c = {
                    texture_texto,
                    glm::vec2(font_face->glyph->bitmap.width, font_face->glyph->bitmap.rows),
                    glm::vec2(font_face->glyph->bitmap_left, font_face->glyph->bitmap_top),
                    font_face->glyph->advance.x
                };

                glifos.insert({ c , glifo_c });
            }
        }

        shader_texto = inicializarShaderTexto();
	}

    unsigned int inicializarShaderTexto() {
        std::string vertexShaderCodigo =
            "#version 330 core\n"

            "layout (location = 0) in vec2 posicion; \n"
            "layout (location = 1) in vec2 texturaST; \n"
            
            "out vec2 coordsTextura; \n"

            "void main() {\n"
            "  gl_Position = vec4(posicion, 0.0f, 1.0f); \n"

            "  coordsTextura = texturaST; \n"
            " }\0";

        std::string fragmentShaderCodigo =
            "#version 330 core\n"

            "out vec4 FragColor; \n"

            "in vec2 coordsTextura; \n"
            "uniform sampler2D datosTextura; \n"

            "void main() {\n"
            "   FragColor = vec4( 1.0, 1.0, 1.0, 1.0 ) * vec4(1.0, 1.0, 1.0, texture(datosTextura, coordsTextura).r) ; \n"
            " }\0";
        //texture(datosTextura, coordsTextura).r


        // Vertex shader
        int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        const char* temporal_vs = vertexShaderCodigo.c_str();
        glShaderSource(vertexShader, 1, &temporal_vs, NULL);
        glCompileShader(vertexShader);

        int exito;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &exito);
        if (exito) {
            std::cout << "Se ha compilado el Vertex Shader de texto\n";
        }
        else {
            std::cout << "Ha fallado el Vertex Shader de texto\n";
        }

        // fragment shader
        int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        const char* temporal_fs = fragmentShaderCodigo.c_str();
        glShaderSource(fragmentShader, 1, &temporal_fs, NULL);
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &exito);
        if (exito) {
            std::cout << "Se ha compilado el Fragment Shader de texto\n";
        }
        else {
            std::cout << "Ha fallado el Fragment Shader de texto\n";
        }

        unsigned int id_programa = glCreateProgram();
        glAttachShader(id_programa, vertexShader);
        glAttachShader(id_programa, fragmentShader);
        glLinkProgram(id_programa);

        return id_programa;
    }

	void actualizar(float delta) override {
		// buscar entre las entidades Componentes "Texto" y los renderiza
	}

	void apagar() override {

	}

    void renderText(std::string texto, float x = 0, float y = 0, float escala = 1) {

        // DIBUJADO - POR TANTO - VAOs, BindTexture, ActiveTexture, Shader, Uniforms
        glUseProgram(shader_texto);

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        x = x / 1920 * escala;
        y = y / 1080 * escala;

        for (std::string::const_iterator c = texto.begin(); c != texto.end(); c++) {
      
            Glifo glifo = glifos[*c];

            // PUNTO ABAJO IZQ
            float pos_x = x + (float)(glifo.bearing.x) / 1920 * escala;
            float pos_y = y + (float)(glifo.bearing.y - glifo.size.y) / 1080 * escala;

            std::cout << "BEARING  " << x << "\n";

            // PUNTO ARRIBA DCHA
            float anchura = (float)glifo.size.x / 1920 * escala;
            float altura = (float)glifo.size.y / 1080 * escala;

            float vertices[] = {
                // pos X                 pos  Y      S    T
                pos_x,              pos_y + altura,  0.0f, 0.0f,  // ARR IZQ   
                pos_x,              pos_y,           0.0f, 1.0f,  // ABJ IZQ
                pos_x + anchura,    pos_y,           1.0f, 1.0f,  // ABJ DCHA

                pos_x,              pos_y + altura,  0.0f, 0.0f,  // ARR IZQ  
                pos_x + anchura,    pos_y,           1.0f, 1.0f,  // ABJ DCHA
                pos_x + anchura,    pos_y + altura,  1.0f, 0.0f, // ARR DCHA
            };

            
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, vertices, GL_DYNAMIC_DRAW);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_TRUE, 4 * sizeof(float), (void*)0); // pos
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, 4 * sizeof(float), (void*)(2 * sizeof(float))); // coordsTexture

            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);

            glBindTexture(GL_TEXTURE_2D, glifo.texture_id);

            glDrawArrays(GL_TRIANGLES, 0, 6);

            //std::cout << *c;

            x += (float)(glifo.advance >> 6) / 1920 * escala;
        }

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }


};