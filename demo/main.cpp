#include "src/Demo.h"
#include "src/Utils.h"
#include "src/components/PlayerControllerComponent.h"

#include <entity/Entity.h>
#include <entity/components/TransformComponent.h>
#include <entity/components/MeshRendererComponent.h>
#include <entity/components/CameraComponent.h>	
#include <entity/components/LightComponent.h>
#include <entity/components/RigidBodyComponent.h>
#include <ui/elements/UILabel.h>
#include <ui/elements/UIPanel.h>
#include <ui/elements/UIButton.h>
#include <ui/elements/UIImage.h>
#include <physics/RigidBody.h>
#include <resources/Mesh.h>
#include <resources/Material.h>
#include <resources/Font.h>

using namespace genesis;

void createMenu(GameContext context)
{
	Rect windowSize = context.display.getWindowSize();
	SharedPtr<Font> font = context.resources.getFont("demo/assets/fonts/arial_16.spritefont");

	UIPanel* menuPanel1 = context.ui.createElement<UIPanel>("menuPanel1");
	menuPanel1->setSize(windowSize);
	menuPanel1->setPosition({0, 0});
	menuPanel1->setColor({1.0f, 1.0f, 1.0f, 1.0f});

	UIPanel* menuPanel2 = context.ui.createElement<UIPanel>("menuPanel2");
	menuPanel2->setSize({500, 600});
	menuPanel2->setPosition(getCenteredPosition(*menuPanel2, windowSize));
	menuPanel2->setColor({0.3f, 0.3f, 0.3f, 0.5f});

	UILabel* title = context.ui.createElement<UILabel>("menuTitle");
	title->setContent("Malo");
	title->setFont(font);
	title->setPosition(getCenteredPosition(*title, windowSize));
	title->setColor({0.0f, 0.0f, 0.0f, 1.0f});
	title->setSize({60, 100});
}

void createWorld(GameContext context)
{
	SharedPtr<Material> brickMaterial = context.resources.getMaterial("demo/assets/materials/brick.json");
	SharedPtr<Mesh> floorMesh = context.resources.getMesh("assets/meshes/terrain.obj", GENESIS_VERTEX_PRESET_NORMAL_MAPPED);
	SharedPtr<Mesh> cubeMesh = context.resources.getMesh("assets/meshes/box.obj", GENESIS_VERTEX_PRESET_NORMAL_MAPPED);

	Entity* floor = context.entities.createEntity("floor");

	auto* floorTransform = floor->createComponent<TransformComponent>();
	floorTransform->setScale({0.5f, 0.5f, 0.5f});

	auto* floorMeshRenderer = floor->createComponent<MeshRendererComponent>();
	floorMeshRenderer->setMesh(floorMesh);
	floorMeshRenderer->setMaterial(brickMaterial);

	auto* floorRigidBody = floor->createComponent<RigidBodyComponent>();
	floorRigidBody->setBody(context.physics.createBox(floorTransform->getPosition(), {50.0f, 0.0f, 50.0f}, PhysicsEngine::MotionType::Static));

	Entity* cube = context.entities.createEntity("cube");

	auto* cubeTransform = cube->createComponent<TransformComponent>();
	cubeTransform->setPosition({0.0f, 2.5f, 0.0f});
	cubeTransform->setRotation({1.01f, 0.0f, 0.0f});

	auto* cubeMeshRenderer = cube->createComponent<MeshRendererComponent>();
	cubeMeshRenderer->setMesh(cubeMesh);
	cubeMeshRenderer->setMaterial(brickMaterial);

	auto* cubeRigidBody = cube->createComponent<RigidBodyComponent>();
	cubeRigidBody->setBody(context.physics.createBox({0.0f, 100.0f, 0.0f}, {0.8f, 0.8f, 0.8f}, PhysicsEngine::MotionType::Dynamic));
	cubeRigidBody->getBody()->addForce({0.0f, -2.0f, 0.0f});

	auto* body = cubeRigidBody->getBody();
	body->setOnContactAddedCallback([body](const RigidBody::ContactAddedData& data) {
		body->setPosition({0.0f, 10.0f, 0.0f});
	});
}

void createLights(GameContext context)
{
	Entity* sun = context.entities.createEntity("sun");

	auto* light = sun->createComponent<LightComponent>();
	light->setType(LightComponent::LightType::Directional);
	light->setColor(Vec3{1.0f, 0.95f, 0.85f});
	light->setIntensity(1.0f);

	auto* transform = sun->getComponent<TransformComponent>();
	transform->setPosition(Vec3{0.0f, 100.0f, 0.0f});
	transform->setRotation(Vec3{-0.785f, 0.523f, 0.0f});
}

void createCamera(GameContext context)
{
	Entity* camera = context.entities.createEntity("camera");
	camera->createComponent<CameraComponent>();
}

void setupScene(Game& game) 
{
	GameContext context = game.getContext();

	createCamera(context);
	createMenu(context);
	createLights(context);
	createWorld(context);
}

int main()
{
	try {
		Demo game({.windowTitle = "Demo", .logLevel = Logger::LogLevel::Info});
		setupScene(game);
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