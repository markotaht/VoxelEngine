#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include "Window.h"
#include <gl/glew.h>
#include <SDL_opengl.h>
#include <gl/GLU.h>
#include <iostream>

#include "InputHandler.h"
#include "PerlinNoise.h"

#include "FileDescriptor.h"
#include "Texture2DArrayDescriptor.h"

#include "NewResourceManager.h"
#include "Texture2D.h"
#include "Texture2DArray.h"
#include "TextureLoader.h"
#include "MaterialLoader.h"
#include "ShaderLoader.h"
#include "Texture2DArrayLoader.h"
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
#include "NewChunk.h"
#include "TagPlayerControlled.h"
#include "PitchYawRotation.h"

#include "ChunkGenerator.h"
#include "ChunkMesher.h"
#include "MoveSystem.h"
#include "LookSystem.h"
#include "VoxelTerrain.h"
#include "PhysicsSystem.h"

#include "RigidBodyComponent.h"
#include "ColliderComponent.h"

Window window;
bool quit = false;
SDL_Event e;
InputHandler& inputHandler = InputHandler::getInstance();

engine::resource::ResourceManager resMan;

engine::entity::Registry registry;
engine::system::RenderSystem renderer;
engine::system::MoveSystem mover;
engine::system::LookSystem looker;
engine::system::PhysicsSystem physics;


std::unique_ptr<engine::render::TextRenderer> textRenderer;
std::unique_ptr<engine::world::voxel::VoxelTerrain> terrain;

engine::entity::Entity camera;

bool loadMedia() {
	resMan.registerLoader(std::make_unique<engine::loader::TextureLoader>());
	resMan.registerLoader(std::make_unique<engine::loader::MaterialLoader>(resMan));
	resMan.registerLoader(std::make_unique<engine::loader::TextMaterialLoader>(resMan));
	resMan.registerLoader(std::make_unique<engine::loader::ShaderLoader>());
	resMan.registerLoader(std::make_unique<engine::loader::FontLoader>());
	resMan.registerLoader(std::make_unique<engine::loader::Texture2DArrayLoader>());

	engine::core::ResourceId<engine::asset::Texture2D> texId = resMan.load<engine::asset::Texture2D>("D:/Visual studio/GameEngine/Debug/UV.png", "UV_Texture");
	engine::core::ResourceId<engine::asset::Texture2DArray> arrayTexId = resMan.load<engine::descriptor::Texture2DArrayDescriptor, engine::asset::Texture2DArray>(
		{
			512,512,
			{
				{"GrassTop", {"D:/Visual studio/VoxelEngine/GrassTop.png"}},
				{"GrassBottom", {"D:/Visual studio/VoxelEngine/GrassBottom.png"}},
				{"GrassSide", {"D:/Visual studio/VoxelEngine/GrassSide.png"}},
				{"UV", {"D:/Visual studio/VoxelEngine/GrassBottom.png"} },
		}}
		, "ArrayTexture");
	engine::core::ResourceId<engine::asset::Font> fontId = resMan.load<engine::asset::Font>("ProggyClean.ttf", "ProggyClean");
	engine::core::ResourceId<engine::asset::ShaderProgram> shaderId = resMan.load<engine::descriptor::ShaderDescriptor, engine::asset::ShaderProgram>({ "vertexLayout.vs", "fragmentShader.frag" }, "BasicShader");
	engine::core::ResourceId<engine::asset::ShaderProgram> textShaderId = resMan.load<engine::descriptor::ShaderDescriptor, engine::asset::ShaderProgram>({ "textShader.vs", "textShader.frag" }, "TextShader");
	engine::core::ResourceId<engine::asset::ShaderProgram> axisShaderId = resMan.load<engine::descriptor::ShaderDescriptor, engine::asset::ShaderProgram>({ "axis.vs", "axis.frag" }, "AxisShader");
	engine::core::ResourceId<engine::asset::Mesh> meshId = resMan.add(std::move(engine::asset::Mesh::makeCube()), "Cube");
	engine::core::ResourceId<engine::asset::Mesh> axisMeshId = resMan.add(std::move(engine::asset::Mesh::makeAxis()), "Axis");
	engine::core::ResourceId<engine::asset::Material> matId = resMan.load<engine::descriptor::MaterialDescriptor, engine::asset::Material>({
		{ "vertexLayout.vs", "fragmentShader.frag" } ,
		{},
		{
			{"albedo", engine::descriptor::FileDescriptor{"D:/Visual studio/GameEngine/Debug/UV.png"}}
		}
		}, "Cube");
	engine::core::ResourceId<engine::asset::Material> arrayMatId = resMan.load<engine::descriptor::MaterialDescriptor, engine::asset::Material>({
		{ "arrayTexture.vs", "arrayTexture.frag" } ,
		{},
		{
			{"atlas", engine::descriptor::Texture2DArrayDescriptor{
			512,512,
			{
				{"GrassTop", {"D:/Visual studio/VoxelEngine/GrassTop.png"}},
				{"GrassBottom", {"D:/Visual studio/VoxelEngine/GrassBottom.png"}},
				{"GrassSide", {"D:/Visual studio/VoxelEngine/GrassSide.png"}},
				{"UV", {"D:/Visual studio/VoxelEngine/GrassBottom.png"} },
		}}}
		}
		}, "ArrayAtlas");
	engine::core::ResourceId<engine::asset::Material> axisMatId = resMan.load<engine::descriptor::MaterialDescriptor, engine::asset::Material>({
	{ "axis.vs", "axis.frag" } ,
	{},
	{}
	}, "Cube");
	engine::core::ResourceId<engine::asset::TextMaterial> textMatId = resMan.load<engine::descriptor::TextMaterialDescriptor, engine::asset::TextMaterial>({
		{ "textShader.vs", "textShader.frag" } ,
		{"ProggyClean.ttf"}
		}, "TextMaterial");

	textRenderer = std::move(std::make_unique<engine::render::TextRenderer>(textMatId));
	terrain = std::move(std::make_unique<engine::world::voxel::VoxelTerrain>(resMan, arrayMatId));

	engine::component::TransformComponent cubeTransform;
	engine::component::MeshRendererComponent meshRenderer {meshId, matId};
	engine::component::RigidBodyComponent cubeRigid;
	cubeRigid.isStatic = true;
	engine::component::ColliderComponent cubeCollider;
	cubeCollider.type = engine::component::ColliderType::AABB;
	cubeCollider.size = glm::vec3{ 1, 1, 1};

	engine::entity::Entity cubeEntity = registry.create();

	registry.addEntity(cubeEntity, cubeTransform, meshRenderer, cubeRigid, cubeCollider);

	engine::component::TransformComponent cubeTransform2;
	cubeTransform2.position.y = 5;
	cubeTransform2.position.x = 1.5f;
	engine::component::MeshRendererComponent meshRenderer2 {meshId, matId};
	engine::component::RigidBodyComponent cubeRigid2;
	cubeRigid2.isStatic = false;
	cubeRigid2.acceleration = { 0,  -0.5f ,0 };
	engine::component::ColliderComponent cubeCollider2;
	cubeCollider2.type = engine::component::ColliderType::AABB;
	cubeCollider2.size = glm::vec3{ 1, 1, 1 };

	engine::entity::Entity cubeEntity2 = registry.create();
	registry.addEntity(cubeEntity2, cubeTransform2, meshRenderer2, cubeRigid2, cubeCollider2);

	engine::component::TransformComponent axisTransform;
	engine::component::MeshRendererComponent axisMeshRenderer {axisMeshId, axisMatId};

	//engine::entity::Entity axisEntity = registry.create();
	//registry.add<engine::component::TransformComponent>(axisEntity, axisTransform);
	//registry.add<engine::component::MeshRendererComponent>(axisEntity, axisMeshRenderer);

	engine::component::TransformComponent cameraTransform;
	engine::component::CameraComponent cameraComponent;
	engine::component::PitchYawRotationComponent pirtchYaw;
	engine::component::TagPlayerControlled tagPlayerControllerd;
	cameraTransform.position.z = 10;
	cameraTransform.position.y = 0;

	camera = registry.create();
	registry.addEntity(camera, cameraTransform, cameraComponent, pirtchYaw, tagPlayerControllerd);

	return true;
}

void close() {

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
			Uint64 NOW = 0;
			Uint64 LAST = SDL_GetPerformanceCounter();

			Uint32 NOW_TICK = 0;
			Uint32 LAST_TICK = SDL_GetTicks();
			double deltaTime = 0;
			//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			float smoothedFps = 60.0f;

			while (!quit) {
				NOW_TICK = SDL_GetTicks();
				float delta = static_cast<float>(NOW_TICK - LAST_TICK);
				LAST_TICK = NOW_TICK;

				NOW = SDL_GetPerformanceCounter();
				deltaTime = (NOW - LAST) / (double)SDL_GetPerformanceFrequency();
				LAST = NOW;

				if (delta <= 0.01f) delta = 0.01f;

				float fps = 1000.f / delta;
				smoothedFps = glm::mix(smoothedFps, fps, 0.1f);

				engine::component::TransformComponent* camTransform = nullptr;
				engine::component::CameraComponent* cam = nullptr;

				camTransform = registry.tryGetComponent<engine::component::TransformComponent>(camera);
				cam = registry.tryGetComponent<engine::component::CameraComponent>(camera);

				engine::system::SystemContext context {registry, &resMan, static_cast<float>(deltaTime)};
				textRenderer->addMessage(std::to_string(static_cast<int>(smoothedFps)) + "FPS");
			//	debugLog->addMessage(std::to_string(1000.f / (NOW_TICK - LAST_TICK)) + "FPS");
			//	debugLog->addMessage(std::to_string(deltaTime) + "s");
			//	float chunkSize = Chunk::CHUNK_WIDTH * BLOCK_WIDTH;
			//	debugLog->addMessage("XYZ:" + std::to_string(mainCamera->transform.getPosition().x) + " " + std::to_string(mainCamera->transform.getPosition().y) + " " + std::to_string(mainCamera->transform.getPosition().x));
			//	debugLog->addMessage("CHUNK: [" + std::to_string(mainCamera->transform.getPosition().x / chunkSize) + " " + std::to_string(mainCamera->transform.getPosition().y /  chunkSize)+ " " + std::to_string(mainCamera->transform.getPosition().z / chunkSize) + "]");

				inputHandler.update();
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
			//	terrain->update(camTransform->position);
			//	terrain->render(camTransform->getInverseMatrix(), cam->getProjectionMatrix());
			//	terrain->finalize();
				physics.update(context);
				renderer.update(context);
				looker.update(context);
				mover.update(context);
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