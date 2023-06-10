#pragma once
#include <gl/glew.h>
#include <SDL_opengl.h>
#include <gl/GLU.h>
#include "Transformation.h"
#include "GenericShaderProgram.h"
#include "StandardMaterial.h"
#include "SceneNode.h"
#include "Camera.h"
#include "MeshRenderer.h";
#include "Texture.h"
#include "ShaderProgram.h"
#include "Mesh.h"
#include "Material.h"
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
class GameObject : public SceneNode
{
protected:
	glm::vec3 dir;
public:

	GameObject();
	GameObject(Mesh* mesh, Material* material, glm::vec3 position);
	GameObject(Mesh* mesh, Material* material);
	~GameObject();

	void update(float deltaTime);
	void render(glm::mat4x4 projectionMatrix, glm::mat4x4 viewMatrix, glm::mat4x4 parentTransform);
	void position(glm::vec3 position);

	static GameObject* createCube();
	static GameObject* createPlane();

	Transformation transform;
	MeshRenderer* meshRenderer;
};

