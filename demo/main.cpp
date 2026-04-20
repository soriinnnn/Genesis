#include "src/Demo.h"
#include <resources/Font.h>
#include <resources/Mesh.h>
#include <resources/Material.h>
#include <entity/components/Light.h>
#include <entity/components/Camera.h>
#include <entity/components/Transform.h>
#include <entity/components/MeshRenderer.h>
#include <entity/components/PlayerController.h>
#include <ui/elements/UILabel.h>

using namespace genesis;

void setupWorld(Game& game) 
{
	auto context = game.getContext();

	SharedPtr<Mesh> mesh = context.resources.getMesh("assets/meshes/scene.obj", GENESIS_VERTEX_PRESET_NORMAL_MAPPED);
	SharedPtr<Material> material = context.resources.getMaterial("demo/assets/materials/statue.json");

	Entity* statue = context.world.createEntity();

	Transform* statueTransform = statue->createComponent<Transform>();
	statueTransform->setScale({0.5, 0.5, 0.5});

	MeshRenderer* statueMesh = statue->createComponent<MeshRenderer>();
	statueMesh->setMesh(mesh);
	statueMesh->setMaterial(material);

	Entity* camera = context.world.createEntity();
	camera->createComponent<Camera>();
	camera->getComponent<Transform>()->setPosition({0, 1, -1});
	camera->createComponent<PlayerController>()->setInputManager(context.input);
	context.world.setCamera(camera);

	Entity* sun = context.world.createEntity();

	Light* sunLight = sun->createComponent<Light>();
	sunLight->setType(Light::LightType::Directional);
	sunLight->setColor(Vec3{1.0f, 0.95f, 0.85f});
	sunLight->setIntensity(1.2f);

	Transform* sunTransform = sun->getComponent<Transform>();
	sunTransform->setPosition(Vec3{0.0f, 100.0f, 0.0f});
	sunTransform->setRotation(Vec3{-0.785f, 0.523f, 0.0f});
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