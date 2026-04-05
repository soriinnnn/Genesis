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
	auto& input = game.getInput();
	auto& world = game.getWorld();
	auto& resources = game.getResources();
	auto& ui = game.getUI();

	SharedPtr<Mesh> mesh = resources.getMesh("assets/meshes/scene.obj", GENESIS_VERTEX_PRESET_NORMAL_MAPPED);
	SharedPtr<Material> material = resources.getMaterial("demo/assets/materials/statue.json");
	SharedPtr<Font> font = resources.getFont("demo/assets/fonts/arial_48.spritefont");

	Entity* statue = world.createEntity();

	Transform* statueTransform = statue->createComponent<Transform>();
	statueTransform->setScale({0.5, 0.5, 0.5});

	MeshRenderer* statueMesh = statue->createComponent<MeshRenderer>();
	statueMesh->setMesh(mesh);
	statueMesh->setMaterial(material);

	Entity* camera = world.createEntity();
	camera->createComponent<Camera>();
	camera->getComponent<Transform>()->setPosition({0, 1, -1});
	camera->createComponent<PlayerController>()->setInputManager(input);
	world.setCamera(camera);

	Entity* sun = world.createEntity();

	Light* sunLight = sun->createComponent<Light>();
	sunLight->setType(Light::LightType::Directional);
	sunLight->setColor(Vec3{1.0f, 0.95f, 0.85f});
	sunLight->setIntensity(1.2f);

	Transform* sunTransform = sun->getComponent<Transform>();
	sunTransform->setPosition(Vec3{0.0f, 100.0f, 0.0f});
	sunTransform->setRotation(Vec3{-0.785f, 0.523f, 0.0f});

	UILabel* label = ui.createElement<UILabel>();
	label->setContent("Malo");
	label->setFont(font);
	label->setColor({0.0f, 0.0f, 0.0f, 1.0f});
	label->setPosition({0, 100});
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