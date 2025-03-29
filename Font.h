#pragma once
#include <unordered_map>
#include <gl/glew.h>
#include <glm/glm.hpp>

#include "NewResource.h"

namespace engine::asset {
	struct Character {
		glm::vec2 uvTopLeft;
		glm::vec2 uvBottomRight; // ID handle of the glyph texture
		glm::ivec2   size;      // Size of glyph
		glm::ivec2   bearing;   // Offset from baseline to left/top of glyph
		unsigned int advance;   // Horizontal offset to advance to next glyph
	};

	class Font : public resource::Resource {
	public:

		Font(GLuint texture, GLuint vao, GLuint vbo, std::unordered_map<GLchar, Character> characters):texture(texture), vao(vao), vbo(vbo), characters(std::move(characters)){}
		~Font();

		void bind();
		void unbind();

		void unload() override {};

		Character getCharater(GLchar c) const { return characters.at(c); }
		GLuint getVbo() const { return vbo; }
		GLuint getVao() const { return vao; }
		GLuint getTextureId() const { return texture; }
	private:
		std::unordered_map<GLchar, Character> characters;
		GLuint texture;
		GLuint vao;
		GLuint vbo;
	};
}