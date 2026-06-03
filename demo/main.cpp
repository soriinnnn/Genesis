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


	/*
	Rect ref = {TARGET_RESOLUTION_WIDTH, TARGET_RESOLUTION_HEIGHT};

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

static void createMainMenu(Game& game)
{
	GameContext context = game.getContext();

	UIImage* menuPanel = context.ui.createElement<UIImage>(UI_MAIN_MENU_PANEL);
	menuPanel->setTexture(context.resources.getTexture(ASSETS_MAIN_MENU_PANEL));
	menuPanel->setColor({1.0f, 1.0f, 1.0f, 0.5f});
	menuPanel->setAnchor(Anchor::Center);
	menuPanel->setVisible(false);

	UILabel* menuTitle = context.ui.createElement<UILabel>(UI_MAIN_MENU_TITLE);
	menuTitle->setFont(context.resources.getFont(ASSETS_FONT_PRIMARY_24_PX));
	menuTitle->setContent("STARFIELD");
	menuTitle->setColor(fontColor);
	menuTitle->setSize({180, 30});
	menuTitle->setAnchor(Anchor::Center);
	menuTitle->setMargin({0, -150});
	menuTitle->setVisible(false);

	/* START BUTTON */

	UIButton* menuStart = context.ui.createElement<UIButton>(UI_MAIN_MENU_START_BUTTON);
	menuStart->setColor(buttonIdleColor);
	menuStart->setSize(buttonSize);
	menuStart->setAnchor(Anchor::Center);
	menuStart->setMargin({0, -38});
	menuStart->setVisible(false);

	menuStart->setOnMouseEnterCallback([context]() {
		context.ui.getElement<UIButton>(UI_MAIN_MENU_START_BUTTON)->setColor(buttonHoverColor);
	});
	menuStart->setOnMouseOutCallback([context]() {
		context.ui.getElement<UIButton>(UI_MAIN_MENU_START_BUTTON)->setColor(buttonIdleColor);
	});

	UILabel* menuStartLabel = menuStart->getLabel();
	menuStartLabel->setFont(context.resources.getFont(ASSETS_FONT_PRIMARY_16_PX));
	menuStartLabel->setContent("START");
	menuStartLabel->setColor(fontColor);
	menuStartLabel->setSize({72, 22});
	menuStartLabel->setAnchor(Anchor::Center);

	/* SETTINGS BUTTON */

	UIButton* menuSettings = context.ui.createElement<UIButton>(UI_MAIN_MENU_SETTINGS_BUTTON);
	menuSettings->setColor(buttonIdleColor);
	menuSettings->setSize(buttonSize);
	menuSettings->setAnchor(Anchor::Center);
	menuSettings->setMargin({0, 50});
	menuSettings->setVisible(false);

	menuSettings->setOnMouseEnterCallback([context]() {
		context.ui.getElement<UIButton>(UI_MAIN_MENU_SETTINGS_BUTTON)->setColor(buttonHoverColor);
	});
	menuSettings->setOnMouseOutCallback([context]() {
		context.ui.getElement<UIButton>(UI_MAIN_MENU_SETTINGS_BUTTON)->setColor(buttonIdleColor);
	});
	menuSettings->setOnMouseDownCallback([context](MouseButton button) {
		if (button != MouseButton::Left) {
			return;
		}
		context.ui.getElement<UIImage>(UI_MAIN_MENU_PANEL)->setVisible(false);
		context.ui.getElement<UILabel>(UI_MAIN_MENU_TITLE)->setVisible(false);
		context.ui.getElement<UIButton>(UI_MAIN_MENU_START_BUTTON)->setVisible(false);
		context.ui.getElement<UIButton>(UI_MAIN_MENU_SETTINGS_BUTTON)->setVisible(false);
		context.ui.getElement<UIButton>(UI_MAIN_MENU_QUIT_BUTTON)->setVisible(false);
		context.ui.getElement<UIImage>(UI_SETTINGS_MENU_PANEL)->setVisible(true);
		context.ui.getElement<UILabel>(UI_SETTINGS_MENU_TITLE)->setVisible(true);
		context.ui.getElement<UIButton>(UI_SETTINGS_MENU_BACK_BUTTON)->setVisible(true);
	});

	UILabel* menuSettingsLabel = menuSettings->getLabel();
	menuSettingsLabel->setFont(context.resources.getFont(ASSETS_FONT_PRIMARY_16_PX));
	menuSettingsLabel->setContent("SETTINGS");
	menuSettingsLabel->setColor(fontColor);
	menuSettingsLabel->setSize({110, 22});
	menuSettingsLabel->setAnchor(Anchor::Center);

	/* QUIT BUTTON */

	UIButton* menuQuit = context.ui.createElement<UIButton>(UI_MAIN_MENU_QUIT_BUTTON);
	menuQuit->setColor(buttonIdleColor);
	menuQuit->setSize(buttonSize);
	menuQuit->setAnchor(Anchor::Center);
	menuQuit->setMargin({0, 137});
	menuQuit->setVisible(false);

	menuQuit->setOnMouseEnterCallback([context]() {
		context.ui.getElement<UIButton>(UI_MAIN_MENU_QUIT_BUTTON)->setColor(buttonHoverColor);
	});
	menuQuit->setOnMouseOutCallback([context]() {
		context.ui.getElement<UIButton>(UI_MAIN_MENU_QUIT_BUTTON)->setColor(buttonIdleColor);
	});
	menuQuit->setOnMouseDownCallback([&](MouseButton button) {
		if (button != MouseButton::Left) {
			return;
		}
		game.quit();
	});

	UILabel* menuQuitLabel = menuQuit->getLabel();
	menuQuitLabel->setFont(context.resources.getFont(ASSETS_FONT_PRIMARY_16_PX));
	menuQuitLabel->setContent("QUIT");
	menuQuitLabel->setColor(fontColor);
	menuQuitLabel->setSize({54, 22});
	menuQuitLabel->setAnchor(Anchor::Center);
}

static void createSettingsMenu(Game& game)
{
	GameContext context = game.getContext();

	UIImage* menuPanel = context.ui.createElement<UIImage>(UI_SETTINGS_MENU_PANEL);
	menuPanel->setTexture(context.resources.getTexture(ASSETS_SETTINGS_MENU_PANEL));
	menuPanel->setColor({1.0f, 1.0f, 1.0f, 0.5f});
	menuPanel->setAnchor(Anchor::Center);
	menuPanel->setVisible(false);

	UILabel* menuTitle = context.ui.createElement<UILabel>(UI_SETTINGS_MENU_TITLE);
	menuTitle->setFont(context.resources.getFont(ASSETS_FONT_PRIMARY_24_PX));
	menuTitle->setContent("SETTINGS");
	menuTitle->setColor(fontColor);
	menuTitle->setSize({164, 30});
	menuTitle->setAnchor(Anchor::Center);
	menuTitle->setMargin({0, -250});
	menuTitle->setVisible(false);

	/* BACK BUTTON */

	UIButton* menuBack = context.ui.createElement<UIButton>(UI_SETTINGS_MENU_BACK_BUTTON);
	menuBack->setColor(buttonIdleColor);
	menuBack->setSize(buttonSize);
	menuBack->setAnchor(Anchor::Center);
	menuBack->setMargin({0, 0});
	menuBack->setVisible(false);

	menuBack->setOnMouseDownCallback([context](MouseButton button) {
		if (button != MouseButton::Left) {
			return;
		}
		context.ui.getElement<UIImage>(UI_MAIN_MENU_PANEL)->setVisible(true);
		context.ui.getElement<UILabel>(UI_MAIN_MENU_TITLE)->setVisible(true);
		context.ui.getElement<UIButton>(UI_MAIN_MENU_START_BUTTON)->setVisible(true);
		context.ui.getElement<UIButton>(UI_MAIN_MENU_SETTINGS_BUTTON)->setVisible(true);
		context.ui.getElement<UIButton>(UI_MAIN_MENU_QUIT_BUTTON)->setVisible(true);
		context.ui.getElement<UIImage>(UI_SETTINGS_MENU_PANEL)->setVisible(false);
		context.ui.getElement<UILabel>(UI_SETTINGS_MENU_TITLE)->setVisible(false);
		context.ui.getElement<UIButton>(UI_SETTINGS_MENU_BACK_BUTTON)->setVisible(false);
	});
	menuBack->setOnMouseEnterCallback([context]() {
		context.ui.getElement<UIButton>(UI_SETTINGS_MENU_BACK_BUTTON)->setColor(buttonHoverColor);
	});
	menuBack->setOnMouseOutCallback([context]() {
		context.ui.getElement<UIButton>(UI_SETTINGS_MENU_BACK_BUTTON)->setColor(buttonIdleColor);
	});

	UILabel* menuBackLabel = menuBack->getLabel();
	menuBackLabel->setFont(context.resources.getFont(ASSETS_FONT_PRIMARY_16_PX));
	menuBackLabel->setContent("BACK");
	menuBackLabel->setColor(fontColor);
	menuBackLabel->setSize({60, 22});
	menuBackLabel->setAnchor(Anchor::Center);

	context.ui.setZOrder(UI_SETTINGS_MENU_PANEL, 1);
	context.ui.setZOrder(UI_SETTINGS_MENU_TITLE, 1);
	context.ui.setZOrder(UI_SETTINGS_MENU_BACK_BUTTON, 1);
}

static void createLights(Game& game)
{
	GameContext context = game.getContext();

	Entity* sun = context.entities.createEntity(ENTITIES_SUN);

	LightComponent* sunLight = sun->createComponent<LightComponent>();
	sunLight->setType(LightComponent::LightType::Directional);
	sunLight->setColor(SUN_COLOR);

	TransformComponent* sunTransform = sun->getComponent<TransformComponent>();
	sunTransform->setRotation(SUN_DIRECTION);
}

void setupGame(Game& game) 
{
	createMainMenu(game);
	createSettingsMenu(game);
	createLights(game);
}

int main()
{
	try {
		Demo game({.windowTitle = "Starfield", .windowIcon = "C:/Users/Sorin/Downloads/draven_bof.ico", .logLevel = Logger::LogLevel::Info});
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