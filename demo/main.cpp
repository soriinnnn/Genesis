#include "src/Demo.h"
#include "src/utils/Macros.h"
#include "src/utils/Constants.h"
#include "src/utils/Utils.h"

#include "src/scripts/PlayerMovement.h"

#include <entity/components/TransformComponent.h>
#include <entity/components/MeshRendererComponent.h>
#include <entity/components/CameraComponent.h>	
#include <entity/components/LightComponent.h>
#include <entity/components/RigidBodyComponent.h>
#include <entity/components/ScriptComponent.h>
#include <ui/elements/UILabel.h>
#include <ui/elements/UIPanel.h>
#include <ui/elements/UIButton.h>
#include <ui/elements/UIImage.h>
#include <physics/RigidBody.h>
#include <resources/Mesh.h>
#include <resources/Material.h>
#include <resources/Font.h>

using namespace genesis;
using namespace utils;
using namespace constants;

void createMainMenu(GameContext context)
{
	Rect ref = {TARGET_RESOLUTION_WIDTH, TARGET_RESOLUTION_HEIGHT};
	/*
	UIPanel* background = context.ui.createElement<UIPanel>(MAIN_MENU_BACKGROUND);
	background->setSize(context.display.getSize());
	background->setPosition({0, 0});
	background->setColor({0.0f, 0.0f, 0.0f, 1.0f});	

	UIImage* earth = context.ui.createElement<UIImage>(MAIN_MENU_EARTH);
	earth->setTexture(context.resources.getTexture(TEXTURE_PATH_EARTH));
	earth->setAnchor(Anchor::Center);
	earth->setMargin(applyMarginScale(earthMargin, getSizeScaleRatio(context.display.getSize(), referenceSize)));
	earth->setScale(getSizeScaleRatio(context.display.getSize(), referenceSize));

	UIImage* moon = context.ui.createElement<UIImage>(MAIN_MENU_MOON);
	*/
	SharedPtr<Font> font = context.resources.getFont(ASSETS_FONT);

	UIImage* menuPanel = context.ui.createElement<UIImage>("menuPanel");
	menuPanel->setTexture(context.resources.getTexture("demo/assets/textures/ui/menu.png"));
	menuPanel->setAnchor(Anchor::Center);
	menuPanel->setColor({1.0f, 1.0f, 1.0f, 0.5f});

	UILabel* title = context.ui.createElement<UILabel>("menuTitle");
	title->setContent("Malo");
	title->setFont(font);
	title->setAnchor(Anchor::Center);
	title->setColor({1.0f, 1.0f, 1.0f, 1.0f});
	title->setSize({60, 100});

	/*
		m_uiManager->getElement<UIPanel>(MAIN_MENU_BACKGROUND)->setSize(size);

		UIImage* menuEarth = m_uiManager->getElement<UIImage>(MAIN_MENU_EARTH);
		menuEarth->setMargin(applyMarginScale(earthMargin, getSizeScaleRatio(size, referenceSize)));
		menuEarth->setScale(getSizeScaleRatio(size, referenceSize));
		*/
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
	floorRigidBody->setBody(context.physics.createBox(floorTransform->getPosition(), {50.0f, 0.0f, 50.0f}, MotionType::Static));

	Entity* cube = context.entities.createEntity("cube");

	auto* cubeTransform = cube->createComponent<TransformComponent>();
	cubeTransform->setPosition({0.0f, 2.5f, 0.0f});
	cubeTransform->setRotation({1.01f, 0.0f, 0.0f});

	auto* cubeMeshRenderer = cube->createComponent<MeshRendererComponent>();
	cubeMeshRenderer->setMesh(cubeMesh);
	cubeMeshRenderer->setMaterial(brickMaterial);

	auto* cubeRigidBody = cube->createComponent<RigidBodyComponent>();
	cubeRigidBody->setBody(context.physics.createBox({0.0f, 100.0f, 0.0f}, {0.8f, 0.8f, 0.8f}, MotionType::Dynamic));
	cubeRigidBody->getBody()->addForce({0.0f, -2.0f, 0.0f});

	auto* body = cubeRigidBody->getBody();
	body->setOnContactAddedCallback([body](const RigidBody::ContactAddedData& data) {
		body->setPosition({0.0f, 10.0f, 0.0f});
	});
}

static void createLights(GameContext context)
{
	Entity* sun = context.entities.createEntity(ENTITIES_SUN);

	LightComponent* light = sun->createComponent<LightComponent>();
	light->setType(LightComponent::LightType::Directional);
	light->setColor(SUN_COLOR);

	TransformComponent* transform = sun->getComponent<TransformComponent>();
	transform->setRotation(SUN_DIRECTION);
}

void setupGame(Game& game) 
{
	GameContext context = game.getContext();
	//createMainMenu(context);
	createLights(context);
}

int main()
{
	try {
		Demo game({.windowTitle = "Demo", .windowIcon = "C:/Users/Sorin/Downloads/draven_bof.ico", .logLevel = Logger::LogLevel::Info});
		setupGame(game);
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