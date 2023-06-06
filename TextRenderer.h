#pragma once

#include <iostream>
#include <map>
#include <string>

#include <gl/glew.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "ShaderProgram.h"
#include "Window.h"

struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};

class TextRenderer
{
public:
	TextRenderer(std::string filename, ShaderProgram* shaderProgram);
	~TextRenderer();

    void render(std::string text, float x, float y, float scale, glm::vec3 color);
private:
    bool init(std::string filename);

    ShaderProgram* shaderProgram;
    std::map<GLchar, Character> Characters;
    GLuint vao;
    GLuint vbo;
};

