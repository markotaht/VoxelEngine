#pragma once
#include "Material.h"
#include "ShaderProgram.h"

#include <gl/glew.h>
#include <SDL_opengl.h>
#include <gl/GLU.h>
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>
class MTLMaterial : public Material
{
	glm::vec3 ambientColor; //Ka
	glm::vec3 diffuseColor; // Kd
	glm::vec3 specularColor; // Ks
	glm::vec3 emissiveColor; // Ke
	GLfloat shininess; // Ns
	GLfloat opacity; // d

public:
	MTLMaterial() {
		opacity = 1;
		shininess = 10.00002;
	};

	void setUniforms();
	void setTextures();

	void setAmbientColor(glm::vec3 color);
	void setDiffuseColor(glm::vec3 color);
	void setSpecularColor(glm::vec3 color);
	void setEmissiveColor(glm::vec3 color);

	void setShininess(GLfloat shininess);
	void setOpacity(GLfloat opacity);
};

