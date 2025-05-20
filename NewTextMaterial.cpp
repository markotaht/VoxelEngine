#include "TextMaterial.h"
#include "ShaderProgram.h"
#include "ResourceManager.h"
#include "Font.h"

namespace engine::asset {
	void TextMaterial::bind(glm::vec3 color, glm::mat4 projection, resource::ResourceManager& resourceManager)
	{
		ShaderProgram* shaderProgram = resourceManager.get<ShaderProgram>(shaderProgramId);
		Font* font = resourceManager.get<Font>(fontId);
		if (!shaderProgram || !font) return;
		shaderProgram->bind();

		GLint projLoc = glGetUniformLocation(shaderProgram->getProgramId(), "projection");
		GLint colorLoc = glGetUniformLocation(shaderProgram->getProgramId(), "textColor");
		GLint loc = glGetUniformLocation(shaderProgram->getProgramId(), "text");

		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniform3fv(colorLoc, 1, glm::value_ptr(color));
		glUniform1i(loc, 0);

		font->bind();

	}

	void TextMaterial::unbind(resource::ResourceManager& resourceManager)
	{
		ShaderProgram* shaderProgram = resourceManager.get<ShaderProgram>(shaderProgramId);
		Font* font = resourceManager.get<Font>(fontId);
		if (!shaderProgram || !font) return;

		shaderProgram->unbind();
		font->unbind();
	}
}
