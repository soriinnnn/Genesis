#include "MainMenu.h"
#include "MainGame.h"
#include "../utils/GameState.h"
#include "../utils/Constants.h"
#include "../utils/Utils.h"

#include <ui/UIManager.h>
#include <ui/elements/UIImage.h>
#include <ui/elements/UILabel.h>
#include <ui/elements/UIButton.h>
#include <resources/ResourceManager.h>
#include <entity/components/TransformComponent.h>
#include <entity/components/MeshRendererComponent.h>
#include <entity/components/ScriptComponent.h>

using namespace constants;
using namespace types;
using namespace utils;

#define INITIAL_SPACESHIP_POSITION Vec3{-20.0f, -3.0f, 15.0f}
#define INITIAL_SPACESHIP_ROTATION Vec3{0.0f, 0.0f, -0.27f}

#define INITIAL_ASTEROID1_POSITION Vec3{12.0f, 2.0f, 0.0f}
#define INITIAL_ASTEROID1_ROTATION Vec3{0.0f, 0.0f, 0.0f}

#define INITIAL_ASTEROID2_POSITION Vec3{-25.0f, 10.0f, 35.0f}
#define INITIAL_ASTEROID2_ROTATION Vec3{0.0f, 0.0f, 0.0f}

#define INITIAL_ASTEROID3_POSITION Vec3{0.0f, -8.0f, 21.0f}
#define INITIAL_ASTEROID3_ROTATION Vec3{0.0f, 0.0f, 0.0f}

MainMenu::MainMenu(const ScriptDesc& desc): Script(desc)
{
	m_spaceship = nullptr;
	m_asteroid1 = nullptr;
	m_asteroid2 = nullptr;
	m_asteroid3 = nullptr;
}

MainMenu::~MainMenu() 
{
	if (m_spaceship) {
		m_context.entities.destroyEntity(m_spaceship->getId());
	}
	if (m_asteroid1) {
		m_context.entities.destroyEntity(m_asteroid1->getId());
	}
	if (m_asteroid2) {
		m_context.entities.destroyEntity(m_asteroid2->getId());
	}
	if (m_asteroid3) {
		m_context.entities.destroyEntity(m_asteroid3->getId());
	}
	setUIVisibility(false, mainMenuElements, m_context.ui);
	setUIVisibility(false, hints, m_context.ui);
}

void MainMenu::onAwake() {}

void MainMenu::onStart()
{
	if (gameState != GameState::MainMenu) {
		GENESIS_LOG_THROW_ERROR("Invalid game state.");
	}

	Entity* camera = m_context.entities.getEntityByName(ENTITIES_MAIN_CAMERA);
	if (!camera) {
		GENESIS_LOG_THROW_ERROR("Entity \"{}\" not found.", ENTITIES_MAIN_CAMERA);
	}

	TransformComponent* transform = camera->getComponent<TransformComponent>();
	transform->setPosition({0.0f, 0.0f, 0.0f});
	transform->setRotation({0.11f, -0.59f, 0.0f});

	setupScene();
	setupMenu();
}

void MainMenu::onUpdate(float deltaTime) 
{
	updateAnimation(m_spaceshipPosAnim, deltaTime);
	updateAnimation(m_spaceshipRotAnim, deltaTime);

	TransformComponent* spaceshipTransform = m_spaceship->getComponent<TransformComponent>();
	spaceshipTransform->setPosition(m_spaceshipPosAnim.target);
	spaceshipTransform->setRotation(m_spaceshipRotAnim.target);

	updateAnimation(m_asteroid1PosAnim, deltaTime);
	updateAnimation(m_asteroid1RotAnim, deltaTime);

	TransformComponent* asteroid1Transform = m_asteroid1->getComponent<TransformComponent>();
	asteroid1Transform->setPosition(m_asteroid1PosAnim.target);
	asteroid1Transform->setRotation(m_asteroid1RotAnim.target);

	updateAnimation(m_asteroid2PosAnim, deltaTime);
	updateAnimation(m_asteroid2RotAnim, deltaTime);

	TransformComponent* asteroid2Transform = m_asteroid2->getComponent<TransformComponent>();
	asteroid2Transform->setPosition(m_asteroid2PosAnim.target);
	asteroid2Transform->setRotation(m_asteroid2RotAnim.target);

	updateAnimation(m_asteroid3RotAnim, deltaTime);

	TransformComponent* asteroid3Transform = m_asteroid3->getComponent<TransformComponent>();
	asteroid3Transform->setRotation(m_asteroid3RotAnim.target);
}

void MainMenu::onFixedUpdate(float deltaTime) {}

void MainMenu::setupScene() 
{
	m_spaceship = m_context.entities.createEntity();

	TransformComponent* spaceshipTransform = m_spaceship->createComponent<TransformComponent>();
	spaceshipTransform->setPosition(INITIAL_SPACESHIP_POSITION);
	spaceshipTransform->setRotation(INITIAL_SPACESHIP_ROTATION);

	MeshRendererComponent* spaceshipMesh = m_spaceship->createComponent<MeshRendererComponent>();
	spaceshipMesh->setMesh(m_context.resources.getMesh(ASSETS_MESH_SPACESHIP, GENESIS_VERTEX_PRESET_NORMAL_MAPPED));
	spaceshipMesh->setMaterial(m_context.resources.getMaterial(ASSETS_MATERIAL_SPACESHIP));

	m_spaceshipPosAnim.target = spaceshipTransform->getPosition();
	m_spaceshipPosAnim.transitions = offsetTransitions(
		mainMenuSpaceshipPosTrans, 
		countof(mainMenuSpaceshipPosTrans), 
		INITIAL_SPACESHIP_POSITION
	);

	m_spaceshipRotAnim.target = spaceshipTransform->getRotation();
	m_spaceshipRotAnim.transitions = offsetTransitions(
		mainMenuSpaceshipRotTrans, 
		countof(mainMenuSpaceshipRotTrans), 
		INITIAL_SPACESHIP_ROTATION
	);

	m_asteroid1 = m_context.entities.createEntity();

	TransformComponent* asteroid1Transform = m_asteroid1->createComponent<TransformComponent>();
	asteroid1Transform->setPosition(INITIAL_ASTEROID1_POSITION);
	asteroid1Transform->setRotation(INITIAL_ASTEROID1_ROTATION);

	MeshRendererComponent* asteroid1Mesh = m_asteroid1->createComponent<MeshRendererComponent>();
	asteroid1Mesh->setMesh(m_context.resources.getMesh(ASSETS_MESH_ASTEROID_1A, GENESIS_VERTEX_PRESET_NORMAL_MAPPED));
	asteroid1Mesh->setMaterial(m_context.resources.getMaterial(ASSETS_MATERIAL_ASTEROID_1A));

	m_asteroid1PosAnim.target = asteroid1Transform->getPosition();
	m_asteroid1PosAnim.transitions = offsetTransitions(
		mainMenuAsteroid1PosTrans,
		countof(mainMenuAsteroid1PosTrans),
		INITIAL_ASTEROID1_POSITION
	);

	m_asteroid1RotAnim.target = asteroid1Transform->getRotation();
	m_asteroid1RotAnim.transitions = offsetTransitions(
		mainMenuAsteroid1RotTrans,
		countof(mainMenuAsteroid1RotTrans),
		INITIAL_ASTEROID1_ROTATION
	);

	m_asteroid2 = m_context.entities.createEntity();

	TransformComponent* asteroid2Transform = m_asteroid2->createComponent<TransformComponent>();
	asteroid2Transform->setPosition(INITIAL_ASTEROID2_POSITION);
	asteroid2Transform->setRotation(INITIAL_ASTEROID2_ROTATION);

	MeshRendererComponent* asteroid2Mesh = m_asteroid2->createComponent<MeshRendererComponent>();
	asteroid2Mesh->setMesh(m_context.resources.getMesh(ASSETS_MESH_ASTEROID_1B, GENESIS_VERTEX_PRESET_NORMAL_MAPPED));
	asteroid2Mesh->setMaterial(m_context.resources.getMaterial(ASSETS_MATERIAL_ASTEROID_1B));

	m_asteroid2PosAnim.target = asteroid2Transform->getPosition();
	m_asteroid2PosAnim.transitions = offsetTransitions(
		mainMenuAsteroid2PosTrans,
		countof(mainMenuAsteroid2PosTrans),
		INITIAL_ASTEROID2_POSITION
	);

	m_asteroid2RotAnim.target = asteroid2Transform->getRotation();
	m_asteroid2RotAnim.transitions = offsetTransitions(
		mainMenuAsteroid2RotTrans,
		countof(mainMenuAsteroid2RotTrans),
		INITIAL_ASTEROID2_ROTATION
	);

	m_asteroid3 = m_context.entities.createEntity();

	TransformComponent* asteroid3Transform = m_asteroid3->createComponent<TransformComponent>();
	asteroid3Transform->setPosition(INITIAL_ASTEROID3_POSITION);
	asteroid3Transform->setRotation(INITIAL_ASTEROID3_ROTATION);
	asteroid3Transform->setScale({2.0f, 2.0f, 2.0f});

	MeshRendererComponent* asteroid3Mesh = m_asteroid3->createComponent<MeshRendererComponent>();
	asteroid3Mesh->setMesh(m_context.resources.getMesh(ASSETS_MESH_ASTEROID_2A, GENESIS_VERTEX_PRESET_NORMAL_MAPPED));
	asteroid3Mesh->setMaterial(m_context.resources.getMaterial(ASSETS_MATERIAL_ASTEROID_2A));

	m_asteroid3RotAnim.target = asteroid3Transform->getRotation();
	m_asteroid3RotAnim.transitions = offsetTransitions(
		mainMenuAsteroid3RotTrans,
		countof(mainMenuAsteroid3RotTrans),
		INITIAL_ASTEROID2_ROTATION
	);
}

void MainMenu::setupMenu()
{
	setUIVisibility(true, mainMenuElements, m_context.ui);
	setUIVisibility(true, hints, m_context.ui);

	UIElement* menuStart = m_context.ui.getElement<UIElement>(UI_MAIN_MENU_START_BUTTON);
	menuStart->setOnMouseUpCallback([this, menuStart](MouseButton button) {
		if (button != MouseButton::Left) {
			return;
		}

		Entity* scripts = m_context.entities.getEntityByName(ENTITIES_GLOBAL_SCRIPTS);
		if (!scripts) {
			GENESIS_LOG_THROW_ERROR("Entity \"{}\" not found.", ENTITIES_GLOBAL_SCRIPTS);
		}

		ScriptComponent* scriptComponent = scripts->getComponent<ScriptComponent>();
		scriptComponent->removeScript(this);

		gameState = GameState::Playing;
		scriptComponent->addScript(m_manager.createScript<MainGame>());

		menuStart->setOnMouseUpCallback(nullptr);
	});
}