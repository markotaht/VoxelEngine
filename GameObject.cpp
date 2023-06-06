#include "GameObject.h"

GameObject::GameObject()
{
	dir = glm::vec3(1, 0, 0);
}

GameObject::GameObject(Texture* texture, ShaderProgram* program, Mesh* mesh)
{
	this->texture = texture;
	dir = glm::vec3(1, 0, 0);
	meshRenderer = new MeshRenderer(mesh, program);
	transform.setRotation(0, glm::vec3(0,1,0));
	transform.translate(glm::vec3(0, -1, 0));
	transform.setScale(glm::vec3(1, 1, 1));
	transform.setPosition(glm::vec3(0, 0, 5));
}

GameObject::GameObject(Texture* texture, ShaderProgram* program, Mesh* mesh, glm::vec3 position)
{
	this->texture = texture;
	dir = glm::vec3(1, 0, 0);
	meshRenderer = new MeshRenderer(mesh, program);
	transform.setRotation(0, glm::vec3(0, 1, 0));
	transform.translate(glm::vec3(0, -1, 0));
	transform.setScale(glm::vec3(1, 1, 1));
	transform.setPosition(position);
}

GameObject::GameObject(Texture* texture, ShaderProgram* program, Mesh* mesh, Material* material)
{
	this->texture = texture;
	dir = glm::vec3(1, 0, 0);
	meshRenderer = new MeshRenderer(mesh, program, material);
	transform.setRotation(1, 0, 0, 0);
	transform.translate(glm::vec3(0, -1, 0));
}

GameObject::~GameObject()
{
	SceneNode::~SceneNode();
	delete meshRenderer;
}

void GameObject::draw(glm::mat4x4 projectionMatrix, glm::mat4x4 viewMatrix, glm::mat4x4 parentTransform)
{
	glm::mat4x4 worldTransform = transform.M() * parentTransform;
	SceneNode::draw(projectionMatrix, viewMatrix, worldTransform);

	if (texture != nullptr) {
		glBindTexture(GL_TEXTURE_2D, texture->getGLindx());
	}
	meshRenderer->bind();

	glUniformMatrix4fv(meshRenderer->getShader()->getUniform("modelMatrix"), 1, GL_FALSE, glm::value_ptr(worldTransform));
	glUniformMatrix4fv(meshRenderer->getShader()->getUniform("viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(meshRenderer->getShader()->getUniform("projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(meshRenderer->getShader()->getUniform("normalMatrix"), 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(worldTransform))));
	glUniform3f(meshRenderer->getShader()->getUniform("lightPos"), 3, 3, -300);
	glUniform3f(meshRenderer->getShader()->getUniform("u_lightDirection"), -3, -3, -5);
	glUniform3f(meshRenderer->getShader()->getUniform("u_ambientLight"), 0.1, 0.1, 0.1);
	meshRenderer->render();
	meshRenderer->unbind();
}

void GameObject::position(glm::vec3 position)
{
	transform.setPosition(position);
}

GameObject* GameObject::createCube()
{
	Texture* texture = new Texture();
	texture->loadFromFile("D:/Visual studio/GameEngine/Debug/UV.png", NULL);
	GenericShaderProgram* shader = new GenericShaderProgram("basic.vs", "basic.fs");
	shader->loadProgram();
	Mesh* mesh = new Mesh();
	mesh->init();
	mesh->cubePrimitive();
	return new GameObject(texture, shader, mesh);
}

GameObject* GameObject::createPlane()
{
	Texture* texture = new Texture();
	texture->loadFromFile("D:/Visual studio/GameEngine/Debug/UV.png", NULL);
	GenericShaderProgram* shader = new GenericShaderProgram("basic.vs", "basic.fs");
	shader->loadProgram();
	Mesh* mesh = new Mesh();
	mesh->init();
	mesh->planePrimitive();
	return new GameObject(texture, shader, mesh);
}

GameObject* GameObject::createCustom(Mesh* mesh, ShaderProgram* shader)
{
	Texture* texture = new Texture();
	texture->loadFromFile("D:/Visual studio/GameEngine/Debug/UV.png", NULL);
	return new GameObject(texture, shader, mesh);
}

void GameObject::update(float deltaTime)
{
	SceneNode::update(deltaTime);
	//transform.rotate(deltaTime, glm::vec3(0, 1, 0));
	//std::cout << transform.getRotation() << std::endl;
	//transform.translate(*dir * deltaTime);
}
