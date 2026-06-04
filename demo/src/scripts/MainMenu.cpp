#include "MainMenu.h"
#include "../utils/Macros.h"
#include "../utils/Constants.h"
#include <entity/components/TransformComponent.h>
#include <entity/components/MeshRendererComponent.h>
#include <resources/ResourceManager.h>
#include <ui/elements/UIImage.h>
#include <ui/elements/UILabel.h>
#include <ui/elements/UIButton.h>
#include <ui/UIManager.h>

#define MAIN_MENU_CAMERA_POSITION Vec3{0.0f, 0.0f, 0.0f}
#define MAIN_MENU_CAMERA_ROTATION Vec3{0.11f, -0.59f, 0.0f}
#define MAIN_MENU_SPACESHIP_POSITION Vec3{-20.0f, -3.0f, 15.0f}
#define MAIN_MENU_SPACESHIP_ROTATION Vec3{0.0f, 0.0f, -0.27f}

using namespace constants;

MainMenu::MainMenu(const ScriptDesc& desc): Script(desc) 
{
	m_camera = nullptr;
	m_spaceship = nullptr;
}

MainMenu::~MainMenu() 
{
	m_context.entities.destroyEntity(m_spaceship->getId());

	for (const String& element : mainMenuElements) {
		UIElement* elem = m_context.ui.getElement<UIElement>(element.c_str());
		elem->setVisible(false);
	}
	for (const String& hint : mainMenuHints) {
		UIElement* elem = m_context.ui.getElement<UIElement>(hint.c_str());
		elem->setVisible(false);
	}
}

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
	setupMenu();
}

void MainMenu::onUpdate(float deltaTime) {}

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

void MainMenu::setupMenu()
{
	for (const String& element : mainMenuElements) {
		UIElement* elem = m_context.ui.getElement<UIElement>(element.c_str());
		elem->setVisible(true);
	}
	for (const String& hint : mainMenuHints) {
		UIElement* elem = m_context.ui.getElement<UIElement>(hint.c_str());
		elem->setVisible(true);
	}
}