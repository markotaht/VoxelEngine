#include "Axis.h"

Axis::Axis(Material* material):material{ material }
{
	init();
}

void Axis::render(glm::mat4x4 projectionMatrix, glm::mat4x4 viewMatrix, glm::mat4x4 parentTransform)
{
	glm::mat4x4 worldTransform = transform.M() * parentTransform;
	meshRenderer->bind();

	glUniformMatrix4fv(meshRenderer->getShader()->getUniform("modelMatrix"), 1, GL_FALSE, glm::value_ptr(worldTransform));
	glUniformMatrix4fv(meshRenderer->getShader()->getUniform("viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(meshRenderer->getShader()->getUniform("projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	meshRenderer->render();
	meshRenderer->unbind();
}

void Axis::init()
{
	axisVerts = new glm::vec3[6]{
		//BACK
		glm::vec3(0,0,0),
		glm::vec3(1,0,0),
		
		glm::vec3(0,0,0),
		glm::vec3(0,1,0),
		
		glm::vec3(0,0,0),
		glm::vec3(0,0,1),
	};

	axisColors = new glm::vec3[6]{
		//BACK
		glm::vec3(1,0,0),
		glm::vec3(1,0,0),

		glm::vec3(0,1,0),
		glm::vec3(0,1,0),

		glm::vec3(0,0,1),
		glm::vec3(0,0,1),
	};


	axisIndices = new GLuint[6] {
		0,1,
		2,3,
		4,5
	};

	Mesh* mesh = new Mesh(axisVerts, axisColors, axisIndices, 6, 6, 6);
	meshRenderer = new MeshRenderer(mesh, material);
	meshRenderer->setRenderMode(GL_LINES);
}
