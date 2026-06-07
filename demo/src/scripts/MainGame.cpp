#include "MainGame.h"
#include "MainMenu.h"
#include "SpaceshipController.h"
#include "../utils/GameState.h"
#include "../utils/Constants.h"
#include "../utils/Macros.h"
#include "../utils/Utils.h"

#include <ui/UIManager.h>
#include <ui/elements/UIElement.h>
#include <input/InputManager.h>
#include <script/ScriptManager.h>
#include <resources/ResourceManager.h>
#include <physics/PhysicsEngine.h>
#include <entity/components/TransformComponent.h>
#include <entity/components/MeshRendererComponent.h>
#include <entity/components/RigidBodyComponent.h>
#include <entity/components/ScriptComponent.h>

using namespace constants;
using namespace utils;

MainGame::MainGame(const ScriptDesc& desc): Script(desc) 
{
	m_spaceship = nullptr;
	m_asteroid = nullptr;
	m_wasMouseLocked = false;
}

MainGame::~MainGame() 
{
	if (m_spaceship) {
		m_context.entities.destroyEntity(m_spaceship->getId());
	}
	if (m_asteroid) {
		m_context.entities.destroyEntity(m_asteroid->getId());
	}
	m_context.input.removeListener(this);
	setUIVisibility(false, gameMenuElements, m_context.ui);
}

void MainGame::onAwake() {}

void MainGame::onStart() 
{
	if (gameState != GameState::Playing) {
		GENESIS_LOG_THROW_ERROR("Invalid game state.");
	}
	m_context.input.addListener(this);

	setupScene();
	setupMenu();
}

void MainGame::onUpdate(float deltaTime) {}

void MainGame::onFixedUpdate(float deltaTime) {}

void MainGame::onKeyDown(Key key) {}

void MainGame::onKeyUp(Key key)
{
	if (key != Key::Escape) {
		return;
	}

	bool mouseLock = false;
	if (gameState == GameState::Playing) {
		gameState = GameState::Paused;
		m_wasMouseLocked = m_context.input.isMouseLocked();
	}
	else {
		gameState = GameState::Playing;
		mouseLock = m_wasMouseLocked;
	}

	m_context.input.setMouseLock(mouseLock);
	m_context.input.setMouseVisibility(!mouseLock);

	bool menuVisible = (gameState == GameState::Paused);
	setUIVisibility(menuVisible, gameMenuElements, m_context.ui);
	setUIVisibility(menuVisible, hints, m_context.ui);
	if (!menuVisible) {
		setUIVisibility(false, settingsMenuElements, m_context.ui);
	}
}

void MainGame::onMouseMove(Point delta, Point pos) {}

void MainGame::onMouseDown(MouseButton button, Point pos) {}

void MainGame::onMouseUp(MouseButton button, Point pos) {}

void MainGame::setupScene()
{
	m_spaceship = m_context.entities.createEntity("main_game_spaceship");

	TransformComponent* spaceshipTransform = m_spaceship->createComponent<TransformComponent>();
	spaceshipTransform->setPosition({0.0f, 0.0f, 0.0f});
	spaceshipTransform->setRotation({0.0f, 0.0f, 0.0f});

	MeshRendererComponent* spaceshipMesh = m_spaceship->createComponent<MeshRendererComponent>();
	spaceshipMesh->setMesh(m_context.resources.getMesh(ASSETS_MESH_SPACESHIP, GENESIS_VERTEX_PRESET_NORMAL_MAPPED));
	spaceshipMesh->setMaterial(m_context.resources.getMaterial(ASSETS_MATERIAL_SPACESHIP));

	RigidBodyComponent* spaceshipBody = m_spaceship->createComponent<RigidBodyComponent>();
	spaceshipBody->setBody(m_context.physics.createBox({0.0f, 0.0f, 0.0f}, {26.0f, 4.4f, 22.0f}, MotionType::Kinematic));

	ScriptComponent* spaceshipScripts = m_spaceship->createComponent<ScriptComponent>();
	spaceshipScripts->addScript(m_manager.createScript<SpaceshipController>());

	m_asteroid = m_context.entities.createEntity("main_game_asteroid1");

	TransformComponent* asteroidTransform = m_asteroid->createComponent<TransformComponent>();
	asteroidTransform->setPosition({0.0f, 0.0f, 5.0f});

	MeshRendererComponent* asteroidMesh = m_asteroid->createComponent<MeshRendererComponent>();
	asteroidMesh->setMesh(m_context.resources.getMesh(ASSETS_MESH_ASTEROID_1A, GENESIS_VERTEX_PRESET_NORMAL_MAPPED));
	asteroidMesh->setMaterial(m_context.resources.getMaterial(ASSETS_MATERIAL_ASTEROID_1A));
}

void MainGame::setupMenu()
{
	UIElement* menuMain = m_context.ui.getElement<UIElement>(UI_GAME_MENU_MAIN_BUTTON);
	menuMain->setOnMouseUpCallback([this, menuMain](MouseButton button) {
		if (button != MouseButton::Left) {
			return;
		}

		Entity* scripts = m_context.entities.getEntityByName(ENTITIES_GLOBAL_SCRIPTS);
		if (!scripts) {
			GENESIS_LOG_THROW_ERROR("Entity \"{}\" not found.", ENTITIES_GLOBAL_SCRIPTS);
		}

		ScriptComponent* scriptComponent = scripts->getComponent<ScriptComponent>();
		scriptComponent->removeScript(this);

		gameState = GameState::MainMenu;
		scriptComponent->addScript(m_manager.createScript<MainMenu>());

		menuMain->setOnMouseUpCallback(nullptr);
	});
}