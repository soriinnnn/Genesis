#include "MainMenu.h"
#include "../utils/Macros.h"
#include <entity/components/TransformComponent.h>
#include <entity/components/MeshRendererComponent.h>
#include <resources/ResourceManager.h>

#define MAIN_MENU_CAMERA_POSITION Vec3{0.0f, 0.0f, 0.0f}
#define MAIN_MENU_CAMERA_ROTATION Vec3{0.11f, -0.59f, 0.0f}
#define MAIN_MENU_SPACESHIP_POSITION Vec3{-20.0f, -3.0f, 15.0f}
#define MAIN_MENU_SPACESHIP_ROTATION Vec3{0.0f, 0.0f, -0.27f}

MainMenu::MainMenu(const ScriptDesc& desc): Script(desc) 
{
	m_camera = nullptr;
	m_spaceship = nullptr;
}

MainMenu::~MainMenu() {}

void MainMenu::onAwake() {}

void MainMenu::onStart()
{
	m_camera = m_context.entities.getEntityByName(ENTITIES_MAIN_CAMERA);
	if (!m_camera) {
		GENESIS_LOG_THROW_ERROR("Main camera not found.");
	}

	TransformComponent* transform = m_camera->getComponent<TransformComponent>();
	transform->setPosition(MAIN_MENU_CAMERA_POSITION);
	transform->setRotation(MAIN_MENU_CAMERA_ROTATION);

	setupScene();
}

void MainMenu::onUpdate(float deltaTime)
{
}

void MainMenu::onFixedUpdate(float deltaTime) {}

void MainMenu::setupScene() 
{
	m_spaceship = m_context.entities.createEntity("main_menu_spaceship");

	TransformComponent* spaceshipTransform = m_spaceship->createComponent<TransformComponent>();
	spaceshipTransform->setPosition(MAIN_MENU_SPACESHIP_POSITION);
	spaceshipTransform->setRotation(MAIN_MENU_SPACESHIP_ROTATION);

	MeshRendererComponent* spaceshipMesh = m_spaceship->createComponent<MeshRendererComponent>();
	spaceshipMesh->setMesh(m_context.resources.getMesh(ASSETS_SPACESHIP_MESH));
	spaceshipMesh->setMaterial(m_context.resources.getMaterial(ASSETS_SPACESHIP_MATERIAL));
}