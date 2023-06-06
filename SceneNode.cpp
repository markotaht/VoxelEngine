#include "SceneNode.h"
#include <stdio.h>
#include <iostream>

SceneNode::SceneNode()
{
	children.reserve(10);
}

SceneNode::~SceneNode()
{
	for (unsigned int i = 0; i < children.size(); ++i) {
		delete children[i];
	}
	children.clear();
}

void SceneNode::addChild(SceneNode* child)
{
	children.push_back(child);
}

void SceneNode::setMainCamera(Camera* mainCamera)
{
	this->mainCamera = mainCamera;
}

void SceneNode::update(float deltaTime)
{
	for (std::vector<SceneNode*>::iterator i = children.begin(); i != children.end(); i++) {
		(*i)->update(deltaTime);
	}
}

void SceneNode::draw(glm::mat4x4 projectionMatrix, glm::mat4x4 viewMatrix, glm::mat4x4 parentTransform)
{
	glm::mat4x4 worldTransform = transform.M() * parentTransform;
	for (std::vector<SceneNode*>::iterator i = children.begin(); i != children.end(); i++) {
		(*i)->draw(projectionMatrix, viewMatrix, worldTransform);
	}
}
