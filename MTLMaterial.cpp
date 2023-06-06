#include "MTLMaterial.h"

void MTLMaterial::setUniforms()
{
	glUniform3fv(shaderProgram->getUniform("diffuse"), 1, glm::value_ptr(diffuseColor));
	glUniform3fv(shaderProgram->getUniform("ambient"), 1, glm::value_ptr(ambientColor));
	glUniform3fv(shaderProgram->getUniform("emissive"), 1, glm::value_ptr(emissiveColor));
	glUniform3fv(shaderProgram->getUniform("specular"), 1, glm::value_ptr(specularColor));

	glUniform1f(shaderProgram->getUniform("shininess"), shininess);
	glUniform1f(shaderProgram->getUniform("opacity"), opacity);

}

void MTLMaterial::setTextures()
{
}

void MTLMaterial::setAmbientColor(glm::vec3 color)
{
	ambientColor = color;
}

void MTLMaterial::setDiffuseColor(glm::vec3 color)
{
	diffuseColor = color;
}

void MTLMaterial::setSpecularColor(glm::vec3 color)
{
	specularColor = color;
}

void MTLMaterial::setEmissiveColor(glm::vec3 color)
{
	emissiveColor = color;
}

void MTLMaterial::setShininess(GLfloat shininess)
{
	this->shininess = shininess;
}

void MTLMaterial::setOpacity(GLfloat opacity)
{
	this->opacity = opacity;
}
