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
#include "ArrayTextureMaterial.h"
#include "TextMaterial.h"
#include <gl/glew.h>

#include "ResourceManager.h"
#include "KeyboardHandler.h"
#include "PerlinNoise.h"

#include "ArrayTexture.h"

#include "TextRenderer.h"
#include "DebugLog.h"
#include "Axis.h"

#include "NewResourceManager.h"
#include "NewTexture.h"
#include "TextureLoader.h"
#include "MaterialLoader.h"
#include "ShaderLoader.h"
#include "TextMaterialLoader.h"
#include "FontLoader.h"
#include "NewMeshRenderer.h"
#include "NewMesh.h"
#include <memory>

#include <glm/glm.hpp>

#include "Registry.h"
#include "Entity.h"
#include "RenderSystem.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "NewTextRenderer.h"
//Texture texture;
Window window;
bool quit = false;
SDL_Event e;
//SceneNode* scene;
Camera* mainCamera;

DebugLog* debugLog;

ResourceManager resourceManager;
KeyboardHandler* keyboardHandler;
ArrayTexture* arrayTexture;
PerlinNoise* perlinNoise;

engine::resource::ResourceManager resMan;

//bool loadMedia() {
//	bool success = true;
//
//	perlinNoise = new PerlinNoise();
//	keyboardHandler = KeyboardHandler::getInstance();
//	uint32_t textureId = resourceManager.AddResource<Texture>("D:/Visual studio/GameEngine/Debug/UV.png");
//	uint32_t shaderId = resourceManager.AddResource<GenericShaderProgram>("basic.vs", "basic.fs");
//	uint32_t textShaderId = resourceManager.AddResource<GenericShaderProgram>("textShader.vs", "textShader.fs");
//	uint32_t axisShaderId = resourceManager.AddResource<GenericShaderProgram>("axis.vs", "axis.fs");
//	TextRenderer* textRenderer = new TextRenderer("ProggyClean.ttf", resourceManager.getResource<GenericShaderProgram>(textShaderId));
//	uint32_t materialId = resourceManager.AddResource<StandardMaterial>("Standard", new StandardMaterial(resourceManager.getResource<Texture>(textureId), resourceManager.getResource<GenericShaderProgram>(shaderId)));
//	uint32_t axisMaterialId = resourceManager.AddResource<TextMaterial>("Text", new TextMaterial(resourceManager.getResource<GenericShaderProgram>(axisShaderId)));
//
//	arrayTexture = new ArrayTexture(&resourceManager);
//	arrayTexture->addTexture("GrassTop", "D:/Visual studio/VoxelEngine/VoxelEngine/GrassTop.png");
//	arrayTexture->addTexture("GrassSide", "D:/Visual studio/VoxelEngine/VoxelEngine/GrassSide.png");
//	arrayTexture->addTexture("GrassBottom", "D:/Visual studio/VoxelEngine/VoxelEngine/GrassBottom.png");
//	arrayTexture->addTexture("UV", "D:/Visual studio/VoxelEngine/VoxelEngine/GrassBottom.png");
//	arrayTexture->init();
//
//	uint32_t arrayTextureShaderId = resourceManager.AddResource<GenericShaderProgram>("arrayTexture.vs", "arrayTexture.fs");
//	uint32_t arrayTextureMaterialId = resourceManager.AddResource<ArrayTextureMaterial>("ArrayTexture", new ArrayTextureMaterial(resourceManager.getResource<Texture>("UV"), resourceManager.getResource<GenericShaderProgram>(arrayTextureShaderId)));
//
//	debugLog = DebugLog::getInstance(textRenderer);
//	mainCamera = new Camera();
//	scene = new SceneNode();
//	scene->addChild(mainCamera);
//
//	Axis* axis = new Axis(resourceManager.getResource<TextMaterial>("Text"));
//	scene->addChild(axis);
//	ChunkManager* chunk = new ChunkManager(resourceManager.getResource<Material>(materialId), mainCamera, &resourceManager, perlinNoise);
//	scene->addChild(chunk);
//	
//	return success;
//}

engine::entity::Registry registry;
engine::system::RenderSystem renderer;

std::unique_ptr<engine::render::TextRenderer> textRenderer;

bool loadMediaNew() {
	resMan.registerLoader(std::make_unique<engine::loader::TextureLoader>());
	resMan.registerLoader(std::make_unique<engine::loader::MaterialLoader>(resMan));
	resMan.registerLoader(std::make_unique<engine::loader::TextMaterialLoader>(resMan));
	resMan.registerLoader(std::make_unique<engine::loader::ShaderLoader>());
	resMan.registerLoader(std::make_unique<engine::loader::FontLoader>());

	engine::core::ResourceId<engine::asset::Texture> texId = resMan.load<engine::asset::Texture>("D:/Visual studio/GameEngine/Debug/UV.png", "UV_Texture");
	engine::core::ResourceId<engine::asset::Font> fontId = resMan.load<engine::asset::Font>("ProggyClean.ttf", "ProggyClean");
	engine::core::ResourceId<engine::asset::ShaderProgram> shaderId = resMan.load<engine::descriptor::ShaderDescriptor, engine::asset::ShaderProgram>({ "vertexLayout.vs", "fragmentShader.frag" }, "Basic");
	engine::core::ResourceId<engine::asset::ShaderProgram> textShaderId = resMan.load<engine::descriptor::ShaderDescriptor, engine::asset::ShaderProgram>({ "textShader.vs", "textShader.frag" }, "TextShader");
	engine::core::ResourceId<engine::asset::Mesh> meshId = resMan.add(std::move(engine::asset::Mesh::makeCube()), "Cube");
	engine::core::ResourceId<engine::asset::Material> matId = resMan.load<engine::descriptor::MaterialDescriptor, engine::asset::Material>({
		{ "vertexLayout.vs", "fragmentShader.frag" } ,
		{},
		{
			{"albedo", {"D:/Visual studio/GameEngine/Debug/UV.png"}}
		}
		}, "Cube");
	engine::core::ResourceId<engine::asset::TextMaterial> textMatId = resMan.load<engine::descriptor::TextMaterialDescriptor, engine::asset::TextMaterial>({
		{ "textShader.vs", "textShader.frag" } ,
		{"ProggyClean.ttf"}
		}, "TextMaterial");

	textRenderer = std::move(std::make_unique<engine::render::TextRenderer>(textMatId));

	engine::component::TransformComponent cubeTransform;
	engine::component::MeshRendererComponent meshRenderer {meshId, matId};

	engine::entity::Entity cubeEntity = registry.create();
	registry.add<engine::component::TransformComponent>(cubeEntity, cubeTransform);
	registry.add<engine::component::MeshRendererComponent>(cubeEntity, meshRenderer);

	engine::component::TransformComponent cameraTransform;
	engine::component::CameraComponent cameraComponent;
	cameraTransform.position.z = 5;

	engine::entity::Entity camera = registry.create();
	registry.add<engine::component::TransformComponent>(camera, cameraTransform);
	registry.add<engine::component::CameraComponent>(camera, cameraComponent);

	return true;
}

void close() {
	//delete scene;
	//delete mainCamera;
	delete keyboardHandler;
	//texture.free();
	delete debugLog;
	delete perlinNoise;

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
		if (!loadMediaNew()) {
			printf("Failed to load media");
		}
		else {
			Uint64 NOW = SDL_GetPerformanceCounter();
			Uint64 LAST = 0;

			Uint32 NOW_TICK = SDL_GetTicks();
			Uint32 LAST_TICK = 0;
			double deltaTime = 0;
			//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

			while (!quit) {
				LAST_TICK = NOW_TICK;
				NOW_TICK = SDL_GetTicks();
				LAST = NOW;
				NOW = SDL_GetPerformanceCounter();
				deltaTime = (NOW - LAST) / (double)SDL_GetPerformanceFrequency();

				engine::system::SystemContext context {registry, &resMan, deltaTime};
				textRenderer->addMessage(std::to_string(1000.f / (NOW_TICK - LAST_TICK)) + "FPS");
			//	debugLog->addMessage(std::to_string(1000.f / (NOW_TICK - LAST_TICK)) + "FPS");
			//	debugLog->addMessage(std::to_string(deltaTime) + "s");
			//	float chunkSize = Chunk::CHUNK_WIDTH * BLOCK_WIDTH;
			//	debugLog->addMessage("XYZ:" + std::to_string(mainCamera->transform.getPosition().x) + " " + std::to_string(mainCamera->transform.getPosition().y) + " " + std::to_string(mainCamera->transform.getPosition().x));
			//	debugLog->addMessage("CHUNK: [" + std::to_string(mainCamera->transform.getPosition().x / chunkSize) + " " + std::to_string(mainCamera->transform.getPosition().y /  chunkSize)+ " " + std::to_string(mainCamera->transform.getPosition().z / chunkSize) + "]");

			//	keyboardHandler->handleKeyboardEvent();
				while (SDL_PollEvent(&e) != 0) {
					if (e.type == SDL_QUIT) {
						quit = true;
					}
				}


				//std::cout << deltaTime << std::endl;
			//	scene->update((float)deltaTime);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				glEnable(GL_DEPTH_TEST);
			//	scene.render(resMan);
				//scene->render(mainCamera->getProjectionMatrix(), mainCamera->getViewMatrix());
			//	renderer.update(context);
				glDisable(GL_DEPTH_TEST);
			//	debugLog->render();
				textRenderer->render(resMan);
				window.update();
			}
		}
	}
	close();
	return 0;
}