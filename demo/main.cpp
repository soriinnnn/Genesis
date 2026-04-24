#include "src/Demo.h"
#include <resources/Font.h>
#include <resources/Mesh.h>
#include <resources/Material.h>
#include <entity/components/Light.h>
#include <entity/components/Camera.h>
#include <entity/components/Transform.h>
#include <entity/components/MeshRenderer.h>
#include <entity/components/PlayerController.h>

using namespace genesis;

void createScene(Game& game)
{
	auto context = game.getContext();

	SharedPtr<Mesh> floorMesh = context.resources.getMesh("assets/meshes/terrain.obj", GENESIS_VERTEX_PRESET_NORMAL_MAPPED);
	SharedPtr<Material> brickMaterial = context.resources.getMaterial("demo/assets/materials/brick.json");

	Entity* floor = context.world.createEntity();

	Transform* floorTransform = floor->createComponent<Transform>();
	floorTransform->setScale({0.5f, 0.5f, 0.5f});

	MeshRenderer* floorMeshRenderer = floor->createComponent<MeshRenderer>();
	floorMeshRenderer->setMesh(floorMesh);
	floorMeshRenderer->setMaterial(brickMaterial);
}

void createLights(Game& game)
{
	auto context = game.getContext();

	Entity* sun = context.world.createEntity();

	Light* sunLight = sun->createComponent<Light>();
	sunLight->setType(Light::LightType::Directional);
	sunLight->setColor(Vec3{1.0f, 0.95f, 0.85f});
	sunLight->setIntensity(1.0f);

	Transform* sunTransform = sun->getComponent<Transform>();
	sunTransform->setPosition(Vec3{0.0f, 100.0f, 0.0f});
	sunTransform->setRotation(Vec3{-0.785f, 0.523f, 0.0f});
}

void createCamera(Game& game)
{
	auto context = game.getContext();

	Entity* camera = context.world.createEntity();
	camera->createComponent<Camera>();
	camera->getComponent<Transform>()->setPosition({0, 1, -1});
	camera->createComponent<PlayerController>()->setInputManager(context.input);
	context.world.setCamera(camera);
}

void setupWorld(Game& game) 
{
	createCamera(game);
	createLights(game);
	createScene(game);
}

int main()
{
	try {
		Demo game({.windowTitle = "Demo", .logLevel = Logger::LogLevel::Info});
		setupWorld(game);
		game.run();
	}
	catch (const std::runtime_error&) {
		return EXIT_FAILURE;
	}
	catch (const std::invalid_argument&) {
		return EXIT_FAILURE;
	}
	catch (const std::exception&) {
		return EXIT_FAILURE;
	}
	catch (...) {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}