#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include "Window.h"
#include "Texture.h"
#include <gl/glew.h>
#include <SDL_opengl.h>
#include <gl/GLU.h>
#include "SceneNode.h"
#include "GameObject.h"
#include <iostream>
#include "Camera.h"
#include "ChunkManager.h"
#include "StandardMaterial.h"
#include <gl/glew.h>

#include "ResourceManager.h"
#include "KeyboardHandler.h"

#include "TextRenderer.h"
//Texture texture;
Window window;
bool quit = false;
SDL_Event e;
SceneNode* scene;
Camera* mainCamera;

TextRenderer* textRenderer;

ResourceManager resourceManager;
KeyboardHandler* keyboardHandler;

bool loadMedia() {
	bool success = true;

	keyboardHandler = KeyboardHandler::getInstance();
	uint32_t textureId = resourceManager.AddResource<Texture>("D:/Visual studio/GameEngine/Debug/UV.png");
	uint32_t shaderId = resourceManager.AddResource<GenericShaderProgram>("basic.vs", "basic.fs");
	uint32_t textShaderId = resourceManager.AddResource<GenericShaderProgram>("textShader.vs", "textShader.fs");
	textRenderer = new TextRenderer("ProggyClean.ttf", resourceManager.getResource<GenericShaderProgram>(textShaderId));
	uint32_t materialId = resourceManager.AddResource<StandardMaterial>("Standard", new StandardMaterial(resourceManager.getResource<Texture>(textureId), resourceManager.getResource<GenericShaderProgram>(shaderId)));
	uint32_t meshId = resourceManager.getCubeMesh();


	mainCamera = new Camera();
	scene = new SceneNode();

	ChunkManager* chunk = new ChunkManager(resourceManager.getResource<GenericShaderProgram>(shaderId), resourceManager.getResource<Material>(materialId));
	scene->addChild(chunk);

/*	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			for (int k = 0; k < 5; k++) {
				Chunk* chunk = new Chunk(resourceManager.getResource<GenericShaderProgram>(shaderId), resourceManager.getResource<Material>(materialId));
				chunk->CreateMesh();
				chunk->position(glm::vec3(i * Chunk::CHUNK_SIZE, j * Chunk::CHUNK_SIZE, k * Chunk::CHUNK_SIZE));
				scene->addChild(chunk);
			}
		}
	}*/

	//scene->addChild(new GameObject(resourceManager.getResource<Texture>(textureId), resourceManager.getResource<GenericShaderProgram>(shaderId), resourceManager.getResource<Mesh>(meshId)));
	scene->addChild(mainCamera);

	return success;
}

void close() {
	delete scene;
	//delete mainCamera;
	delete keyboardHandler;
	//texture.free();

	window.~Window();

	IMG_Quit();
	SDL_Quit();
}
int main(int argc, char* args[])
{
	if (!window.init()) {
		printf("Failed to initiate\n");
	}
	else {
		if (!loadMedia()) {
			printf("Failed to load media");
		}
		else {
			Uint64 NOW = SDL_GetPerformanceCounter();
			Uint64 LAST = 0;

			Uint32 NOW_TICK = SDL_GetTicks();
			Uint32 LAST_TICK = 0;
			double deltaTime = 0;
			while (!quit) {
				LAST_TICK = NOW_TICK;
				NOW_TICK = SDL_GetTicks();
				LAST = NOW;
				NOW = SDL_GetPerformanceCounter();
				deltaTime = (NOW - LAST) / (double)SDL_GetPerformanceFrequency();

				keyboardHandler->handleKeyboardEvent();
				while (SDL_PollEvent(&e) != 0) {
					if (e.type == SDL_QUIT) {
						quit = true;
					}
				}


				//std::cout << deltaTime << std::endl;
				scene->update(deltaTime);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				glEnable(GL_DEPTH_TEST);
				scene->render(mainCamera->getProjectionMatrix(), mainCamera->getViewMatrix());
				glDisable(GL_DEPTH_TEST);
				textRenderer->render(std::to_string(1000.f / (NOW_TICK - LAST_TICK)) + "FPS", 100, 100, 0.5f, glm::vec3(0.5f, 0.8f, 0.2f));
				textRenderer->render(std::to_string(deltaTime) + "s" , 100, 150, 0.5f, glm::vec3(0.5f, 0.8f, 0.2f));
				window.update();
			}
		}
	}
	close();
	return 0;
}