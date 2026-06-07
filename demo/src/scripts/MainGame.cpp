#include "MainGame.h"
#include "MainMenu.h"
#include "SpaceshipController.h"
#include "../utils/GameState.h"
#include "../utils/Constants.h"
#include "../utils/Macros.h"

#include <ui/UIManager.h>
#include <ui/elements/UIElement.h>
#include <input/InputManager.h>
#include <script/ScriptManager.h>
#include <resources/ResourceManager.h>
#include <entity/components/TransformComponent.h>
#include <entity/components/MeshRendererComponent.h>
#include <entity/components/ScriptComponent.h>

using namespace constants;

MainGame::MainGame(const ScriptDesc& desc): Script(desc) 
{
	m_spaceship = nullptr;
}

MainGame::~MainGame() 
{
	if (m_spaceship) {
		m_context.entities.destroyEntity(m_spaceship->getId());
	}

	m_context.input.removeListener(this);

	for (const String& element : gameMenuElements) {
		UIElement* elem = m_context.ui.getElement<UIElement>(element.c_str());
		if (elem) {
			elem->setVisible(false);
		}
	}
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

	m_context.input.setMouseLock(false);
	m_context.input.setMouseVisibility(true);

	if (gameState == GameState::Playing) {
		gameState = GameState::Paused;
	}
	else {
		gameState = GameState::Playing;
	}

	bool menuVisible = (gameState == GameState::Paused);

	for (const String& element : gameMenuElements) {
		UIElement* elem = m_context.ui.getElement<UIElement>(element.c_str());
		if (!elem) {
			GENESIS_LOG_THROW_ERROR("UI element \"{}\" not found.", element.c_str());
		}
		elem->setVisible(menuVisible);
	}

	for (const String& element : mainMenuHints) {
		UIElement* elem = m_context.ui.getElement<UIElement>(element.c_str());
		if (!elem) {
			GENESIS_LOG_THROW_ERROR("UI element \"{}\" not found.", element.c_str());
		}
		elem->setVisible(menuVisible);
	}

	if (!menuVisible) {
		for (const String& element : settingsMenuElements) {
			UIElement* elem = m_context.ui.getElement<UIElement>(element.c_str());
			if (!elem) {
				GENESIS_LOG_THROW_ERROR("UI element \"{}\" not found.", element.c_str());
			}
			elem->setVisible(false);
		}
	}
}

void MainGame::onMouseMove(Point delta, Point pos) {}

void MainGame::onMouseDown(MouseButton button, Point pos) {}

void MainGame::onMouseUp(MouseButton button, Point pos) {}

void MainGame::setupScene()
{
	m_spaceship = m_context.entities.createEntity("main_game_spaceship");

	TransformComponent* spaceshipComponent = m_spaceship->createComponent<TransformComponent>();
	spaceshipComponent->setPosition({0.0f, 0.0f, 0.0f});
	spaceshipComponent->setRotation({0.0f, 0.0f, 0.0f});

	MeshRendererComponent* spaceshipMesh = m_spaceship->createComponent<MeshRendererComponent>();
	spaceshipMesh->setMesh(m_context.resources.getMesh(ASSETS_MESH_SPACESHIP, GENESIS_VERTEX_PRESET_NORMAL_MAPPED));
	spaceshipMesh->setMaterial(m_context.resources.getMaterial(ASSETS_MATERIAL_SPACESHIP));

	ScriptComponent* spaceshipScripts = m_spaceship->createComponent<ScriptComponent>();
	spaceshipScripts->addScript(m_manager.createScript<SpaceshipController>());
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