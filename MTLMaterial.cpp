//#include "MTLMaterial.h"
//
//void MTLMaterial::setUniforms()
//{
//	glUniform3fv(shaderProgram->getUniform("diffuse"), 1, glm::value_ptr(diffuseColor));
//	glUniform3fv(shaderProgram->getUniform("ambient"), 1, glm::value_ptr(ambientColor));
//	glUniform3fv(shaderProgram->getUniform("emissive"), 1, glm::value_ptr(emissiveColor));
//	glUniform3fv(shaderProgram->getUniform("specular"), 1, glm::value_ptr(specularColor));
//
//	glUniform1f(shaderProgram->getUniform("shininess"), shininess);
//	glUniform1f(shaderProgram->getUniform("opacity"), opacity);
//
//}
//
//void MTLMaterial::setTextures()
//{
//}
//
//void MTLMaterial::init(Mesh* mesh, GLuint* vao)
//{
//	glGenVertexArrays(1, vao);
//	glBindVertexArray(*vao);
//
//	glBindBuffer(GL_ARRAY_BUFFER, mesh->getVertexBufferId());
//	GLuint posId = shaderProgram->getAttribute("VertexPosition");
//	glVertexAttribPointer(posId, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
//	glEnableVertexAttribArray(posId);
//
//	glBindBuffer(GL_ARRAY_BUFFER, mesh->getNormalBufferId());
//	GLuint normId = shaderProgram->getAttribute("VertexNormal");
//	glVertexAttribPointer(normId, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
//	glEnableVertexAttribArray(normId);
//	
//	glBindBuffer(GL_ARRAY_BUFFER, mesh->getUVBufferId());
//	GLuint uvId = shaderProgram->getAttribute("VertexUV");
//	glVertexAttribPointer(uvId, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
//	glEnableVertexAttribArray(uvId);
//
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->getIndexBufferId());
//
//	glBindVertexArray(0);
//}
//
//void MTLMaterial::setAmbientColor(glm::vec3 color)
//{
//	ambientColor = color;
//}
//
//void MTLMaterial::setDiffuseColor(glm::vec3 color)
//{
//	diffuseColor = color;
//}
//
//void MTLMaterial::setSpecularColor(glm::vec3 color)
//{
//	specularColor = color;
//}
//
//void MTLMaterial::setEmissiveColor(glm::vec3 color)
//{
//	emissiveColor = color;
//}
//
//void MTLMaterial::setShininess(GLfloat shininess)
//{
//	this->shininess = shininess;
//}
//
//void MTLMaterial::setOpacity(GLfloat opacity)
//{
//	this->opacity = opacity;
//}
