#include "GameObject.h"

GameObject::GameObject()
{
	dir = glm::vec3(1, 0, 0);
	meshRenderer = nullptr;
}

GameObject::GameObject(Mesh* mesh, Material* material, glm::vec3 position)
{
	dir = glm::vec3(1, 0, 0);
	meshRenderer = new MeshRenderer(mesh, material);
	transform.setRotation(0, glm::vec3(0, 1, 0));
	transform.translate(glm::vec3(0, -1, 0));
	transform.setScale(glm::vec3(1, 1, 1));
	transform.setPosition(position);
}

GameObject::GameObject(Mesh* mesh, Material* material)
{
	dir = glm::vec3(1, 0, 0);
	meshRenderer = new MeshRenderer(mesh, material);
	transform.setRotation(1, 0, 0, 0);
	transform.translate(glm::vec3(0, -1, 0));
}

GameObject::~GameObject()
{
	if (meshRenderer != nullptr) {
		delete meshRenderer;
	}
}

void GameObject::render(glm::mat4x4 projectionMatrix, glm::mat4x4 viewMatrix, glm::mat4x4 parentTransform)
{
	glm::mat4x4 worldTransform = transform.M() * parentTransform;
	SceneNode::render(projectionMatrix, viewMatrix, worldTransform);

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
	Material* material = new StandardMaterial(texture, shader);
	Mesh* mesh = new Mesh();
	mesh->init();
	mesh->cubePrimitive();
	return new GameObject(mesh, material);
}

GameObject* GameObject::createPlane()
{
	Texture* texture = new Texture();
	texture->loadFromFile("D:/Visual studio/GameEngine/Debug/UV.png", NULL);
	GenericShaderProgram* shader = new GenericShaderProgram("basic.vs", "basic.fs");
	shader->loadProgram();
	Material* material = new StandardMaterial(texture, shader);
	Mesh* mesh = new Mesh();
	mesh->init();
	mesh->planePrimitive();
	return new GameObject(mesh, material);
}

void GameObject::update(float deltaTime)
{
	SceneNode::update(deltaTime);
	//transform.rotate(deltaTime, glm::vec3(0, 1, 0));
	//std::cout << transform.getRotation() << std::endl;
	//transform.translate(*dir * deltaTime);
}
