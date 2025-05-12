#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include "Window.h"
#include <gl/glew.h>
#include <SDL_opengl.h>
#include <gl/GLU.h>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

#include "InputHandler.h"
#include "PerlinNoise.h"

#include "FileDescriptor.h"
#include "Texture2DArrayDescriptor.h"
#include "TextMaterialDescriptor.h"
#include "MaterialDescriptor.h"

#include "ResourceManager.h"
#include "Texture2D.h"
#include "Texture2DArray.h"
#include "NewMeshRenderer.h"
#include "NewMesh.h"
#include "NewMaterial.h"
#include <memory>

#include <glm/glm.hpp>

#include "Entity.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "NewTextRenderer.h"
#include "TagPlayerControlled.h"
#include "PitchYawRotation.h"

#include "VoxelTerrainBackend.h"

#include "RigidBodyComponent.h"
#include "ColliderComponent.h"
#include "SystemRegistry.h"

#include "Scene.h"
#include "SceneSystemRunner.h"
#include "FrameTimer.h"

#include "TerrainSystem.h"
#include "CameraData.h"

#include "ThreadPool.h"
#include "MainThreadDispatcher.h"

Window window;
bool quit = false;
SDL_Event e;
InputHandler& inputHandler = InputHandler::getInstance();

engine::resource::ResourceManager& resMan = engine::resource::ResourceManager::instance();
engine::system::SystemRegistry& sysReg = engine::system::SystemRegistry::instance();

engine::scene::Scene globalScene;
engine::scene::Scene localScene;


std::unique_ptr<engine::render::TextRenderer> textRenderer;
engine::world::TerrainSystem* terrainSystem = nullptr;

engine::entity::Entity camera;

engine::core::ThreadPool threadPool {5};
engine::core::MainThreadDispatcher mainThreadDispatcher;

bool loadMedia() {
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

	//physicsSystem.setTerrainCollision(terrain.get());

	terrainSystem = globalScene.addSubsystem<engine::world::TerrainSystem>();
	terrainSystem->setBackend(std::make_unique<engine::world::voxel::VoxelTerrainBackend>(resMan, arrayMatId, threadPool, mainThreadDispatcher));

	engine::component::TransformComponent cubeTransform;
	engine::component::MeshRendererComponent meshRenderer {meshId, matId};
	engine::component::RigidBodyComponent cubeRigid;
	cubeRigid.isStatic = true;
	engine::component::ColliderComponent cubeCollider;
	cubeCollider.type = engine::component::ColliderType::AABB;
	cubeCollider.size = glm::vec3{ 1, 1, 1};

	//engine::entity::Entity cubeEntity = globalScene.createEntity();

	//registry.addEntity(cubeEntity, cubeTransform, meshRenderer, cubeRigid, cubeCollider);

	engine::component::TransformComponent cubeTransform2;
	cubeTransform2.position.y = 70;
	cubeTransform2.position.x = 1.0f;
	cubeTransform2.position.z = -1;
	engine::component::MeshRendererComponent meshRenderer2 {meshId, matId};
	engine::component::RigidBodyComponent cubeRigid2;
	cubeRigid2.isStatic = false;
	cubeRigid2.velocity = { 0,  -4 ,0 };
	engine::component::ColliderComponent cubeCollider2;
	cubeCollider2.type = engine::component::ColliderType::AABB;
	cubeCollider2.size = glm::vec3{ 1, 1, 1 };

	//engine::entity::Entity cubeEntity2 = registry.create();
	//registry.addEntity(cubeEntity2, cubeTransform2, meshRenderer2, cubeRigid2, cubeCollider2);

	engine::component::TransformComponent axisTransform;
	engine::component::MeshRendererComponent axisMeshRenderer {axisMeshId, axisMatId};

	//engine::entity::Entity axisEntity = registry.create();
	//registry.add<engine::component::TransformComponent>(axisEntity, axisTransform);
	//registry.add<engine::component::MeshRendererComponent>(axisEntity, axisMeshRenderer);

	engine::component::TransformComponent cameraTransform;
	engine::component::CameraComponent cameraComponent;
	engine::component::PitchYawRotationComponent pirtchYaw {-6.5f, 0};
	engine::component::TagPlayerControlled tagPlayerControllerd;

	engine::component::RigidBodyComponent camRigid;
	camRigid.isStatic = false;
	camRigid.acceleration = { 0,  -4 ,0 };
	engine::component::ColliderComponent camCollider;
	camCollider.type = engine::component::ColliderType::AABB;
	camCollider.size = glm::vec3{ 1, 1, 1 };
	cameraTransform.position.z = 10;
	cameraTransform.position.y = 70;

	camera = globalScene.createEntity();
	globalScene.addEntity(camera, cameraTransform, cameraComponent, pirtchYaw, tagPlayerControllerd, camCollider);

	terrainSystem->streamChunksAround(cameraTransform.position);

	return true;
}

void close() {

	window.~Window();

	IMG_Quit();
	SDL_Quit();
}

engine::core::CameraData getActiveCameraData(engine::scene::Scene& scene, engine::entity::Entity camEntity) {
	return {
		scene.tryGetComponent<engine::component::TransformComponent>(camEntity),
		scene.tryGetComponent<engine::component::CameraComponent>(camEntity)
	};
}

void printDebugData(engine::render::TextRenderer* textRenderer, engine::component::TransformComponent* camTransform, engine::core::FrameTimer& timer) {
	textRenderer->addMessage("DeltaTime: " + std::to_string(timer.getDeltaTime()));
	textRenderer->addMessage(std::to_string(static_cast<int>(timer.getSmoothedFPS())) + "FPS");
	textRenderer->addMessage("Camera position: " + glm::to_string(camTransform->position));
	textRenderer->addMessage("Chunk: " + glm::to_string(glm::floor(camTransform->position / engine::world::voxel::Chunk::chunkSize)));
	textRenderer->addMessage("ChunkRaw: " + glm::to_string(camTransform->position / engine::world::voxel::Chunk::chunkSize));
}

int main(int argc, char* args[])
{
	if (!window.init()) {
		std::cerr << "Failed to initialize window\n";
		return -1;
	}

	if (!loadMedia()) {
		std::cerr << "Failed to load media\n";
		return -1;
	}

	engine::system::SceneSystemRunner systemsRunner {sysReg, resMan, globalScene};
	std::vector<engine::scene::Scene*> loadedScenes = { &localScene };

	engine::core::FrameTimer timer;

	while (!quit) {
		timer.update();

		engine::core::CameraData camData = getActiveCameraData(globalScene, camera);
		if (!camData.camera || !camData.transform) {
			continue; // skip frame
		}

		printDebugData(textRenderer.get(), camData.transform, timer);

		inputHandler.update();
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			inputHandler.handleEvent(e);
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		terrainSystem->streamChunksAroundThrottled(camData.transform->position);
		mainThreadDispatcher.runMainThreadTasks();

		while (timer.shouldRunFixedUpdate()) {
			systemsRunner.fixedUpdateAll(timer.getFixedTimeStep(), loadedScenes, camData);
		}

		systemsRunner.updateAll(timer.getDeltaTime(), loadedScenes, camData);

		glDisable(GL_DEPTH_TEST);
		textRenderer->render(resMan);
		window.update();
	}

	close();
	return 0;
}