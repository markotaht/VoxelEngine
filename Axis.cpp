#include "Axis.h"

Axis::Axis(ShaderProgram* shaderProgram):shaderProgram{shaderProgram}
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
	axisVerts = new GLfloat[18]{
		//BACK
		0.f,0.f,0.f,
		1.f,0.f,0.f,

		0.f,0.f,0.f,
		0.f,1.f,0.f,

		0.f,0.f,0.f,
		0.f,0.f,1.f
	};

	axisColors = new GLfloat[18]{
		//BACK
		1.f,0.f,0.f,
		1.f,0.f,0.f,

		0.f,1.f,0.f,
		0.f,1.f,0.f,

		0.f,0.f,1.f,
		0.f,0.f,1.f
	};


	axisIndices = new GLuint[6] {
		0,1,
		2,3,
		4,5
	};

	Mesh* mesh = new Mesh(axisVerts, axisColors, axisIndices, 18, 18, 6);
	meshRenderer = new MeshRenderer(mesh, shaderProgram);
	meshRenderer->setRenderMode(GL_LINES);
}
