#pragma once
#include<vector>
#include <glm/mat4x4.hpp>
#include"Transformation.h"

class Camera;
class SceneNode
{
	std::vector<SceneNode*> children;
protected:
	Camera* mainCamera;
public:
	SceneNode();
	~SceneNode();

	void addChild(SceneNode* child);
	void setMainCamera(Camera* mainCamera);

	virtual void update(float deltaTime);
	virtual void draw(glm::mat4x4 projectionMatrix, glm::mat4x4 viewMatrix, glm::mat4x4 parentTransform = glm::mat4(1.0f));

	Transformation transform;
};

