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
#include <resources/Mesh.h>
#include <physics/PhysicsEngine.h>
#include <entity/components/TransformComponent.h>
#include <entity/components/MeshRendererComponent.h>
#include <entity/components/RigidBodyComponent.h>
#include <entity/components/ScriptComponent.h>

using namespace constants;
using namespace utils;

#define ASTEROID_COUNT 1023
#define ASTEROID_BASE_MASS 2000.0f
#define ASTEROID_SPAWN_RADIUS 2000.0f
#define ASTEROID_MAX_SCALE 50.0f
#define ASTEROID_MIN_SCALE 2.0f
#define ASTEROID_MAX_SPIN 0.2f

MainGame::MainGame(const ScriptDesc& desc): Script(desc) 
{
	m_spaceship = nullptr;
	m_wasMouseLocked = false;
}

MainGame::~MainGame() 
{
	if (m_spaceship) {
		m_context.entities.destroyEntity(m_spaceship->getId());
	}
	for (auto* asteroid : m_asteroids) {
		m_context.entities.destroyEntity(asteroid->getId());
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
	m_spaceship = m_context.entities.createEntity();

	TransformComponent* spaceshipTransform = m_spaceship->createComponent<TransformComponent>();
	spaceshipTransform->setPosition({0.0f, 0.0f, 0.0f});
	spaceshipTransform->setRotation({0.0f, 0.0f, 0.0f});

	MeshRendererComponent* spaceshipMesh = m_spaceship->createComponent<MeshRendererComponent>();

	spaceshipMesh->setMesh(m_context.resources.getMesh(ASSETS_MESH_SPACESHIP, GENESIS_VERTEX_PRESET_NORMAL_MAPPED));
	spaceshipMesh->setMaterial(m_context.resources.getMaterial(ASSETS_MATERIAL_SPACESHIP));

	Vec3 size = m_context.resources.getMesh(ASSETS_MESH_SPACESHIP, GENESIS_VERTEX_PRESET_NORMAL_MAPPED)->getSize();
	SharedPtr<RigidBody> body = m_context.physics.createBox(
		size,
		{Vec3{0.0f, 0.0f, 0.0f}, MotionType::Kinematic, 1.0f}
	);

	RigidBodyComponent* spaceshipBody = m_spaceship->createComponent<RigidBodyComponent>();
	spaceshipBody->setBody(body);

	ScriptComponent* spaceshipScripts = m_spaceship->createComponent<ScriptComponent>();
	spaceshipScripts->addScript(m_manager.createScript<SpaceshipController>());

	for (int i = 0; i < ASTEROID_COUNT; i++) {
		int asteroid = range(0, static_cast<int>(asteroidMeshes.size() - 1));
		spawnAsteroid(
			getRandomVector3(ASTEROID_SPAWN_RADIUS), 
			range(ASTEROID_MIN_SCALE, ASTEROID_MAX_SCALE), 
			asteroidMeshes[asteroid].c_str(),
			asteroidMaterials[asteroid].c_str()
		);
	}
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

void MainGame::spawnAsteroid(Vec3 position, float scale, const char* mesh, const char* material)
{
	Entity* asteroid = m_context.entities.createEntity();

	TransformComponent* asteroidTransform = asteroid->createComponent<TransformComponent>();
	asteroidTransform->setScale(Vec3{1.0f, 1.0f, 1.0f} * scale);

	MeshRendererComponent* asteroidMesh = asteroid->createComponent<MeshRendererComponent>();
	asteroidMesh->setMesh(m_context.resources.getMesh(mesh, GENESIS_VERTEX_PRESET_NORMAL_MAPPED));
	asteroidMesh->setMaterial(m_context.resources.getMaterial(material));

	Vec3 size = m_context.resources.getMesh(mesh, GENESIS_VERTEX_PRESET_NORMAL_MAPPED)->getSize() * scale;
	SharedPtr<RigidBody> body = m_context.physics.createBox(
		size,
		{position, MotionType::Dynamic, ASTEROID_BASE_MASS * scale}
	);
	body->setGravityFactor(0.0f);
	body->setAngularVelocity(getRandomVector3({ASTEROID_MAX_SPIN, ASTEROID_MAX_SPIN, ASTEROID_MAX_SPIN}));

	RigidBodyComponent* asteroidBody = asteroid->createComponent<RigidBodyComponent>();
	asteroidBody->setBody(body);

	m_asteroids.push_back(asteroid);
}