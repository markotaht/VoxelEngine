#include "Font.h"
#include <iostream>

engine::asset::Font::~Font()
{
	unload();
}

void engine::asset::Font::bind()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(vao);
	glBindTexture(GL_TEXTURE_2D, texture);
}

void engine::asset::Font::unbind()
{
	//glBindVertexArray(0);
	//glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_BLEND);
}
