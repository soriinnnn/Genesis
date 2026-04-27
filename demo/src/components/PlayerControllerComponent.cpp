#include "PlayerControllerComponent.h"
#include <entity/components/TransformComponent.h>
#include <entity/Entity.h>
#include <input/InputManager.h>
#include <core/utils/Macros.h>

using namespace genesis;

const float PlayerControllerComponent::MAX_PITCH = 1.553f;
const float PlayerControllerComponent::MOVE_SPEED = 1.0f;
const float PlayerControllerComponent::MOUSE_SENSITIVITY_X = 0.001f;
const float PlayerControllerComponent::MOUSE_SENSITIVITY_Y = 0.001f;

PlayerControllerComponent::PlayerControllerComponent(const ComponentDesc& desc): Component(desc), m_inputManager{nullptr} {}

PlayerControllerComponent::~PlayerControllerComponent() {}

void PlayerControllerComponent::update(float deltaTime)
{
	GENESIS_ASSERT(m_inputManager != nullptr, "InputManager is null.");
	if (!m_inputManager->isMouseLocked()) {
		return;
	}

	TransformComponent* transform = m_entity.getComponent<TransformComponent>();
	GENESIS_ASSERT(transform != nullptr, "PlayerController requires a Transform component.");

	Vec3 position = transform->getPosition();
	Vec3 rotation = transform->getRotation();

	Point delta = m_inputManager->getMouseDelta();
	rotation.x += delta.y * MOUSE_SENSITIVITY_X;
	rotation.y += delta.x * MOUSE_SENSITIVITY_Y;
	if (rotation.x > MAX_PITCH) rotation.x = MAX_PITCH;
	if (rotation.x < -MAX_PITCH) rotation.x = -MAX_PITCH;

	Vec3 forward = transform->getForwardVector();
	Vec3 right = Vec3::normalize(Vec3::cross(Vec3{0.0f, 1.0f, 0.0f}, forward));

	if (m_inputManager->isKeyDown(Key::W)) {
		position += forward * MOVE_SPEED * deltaTime;
	}
	if (m_inputManager->isKeyDown(Key::S)) {
		position -= forward * MOVE_SPEED * deltaTime;
	}
	if (m_inputManager->isKeyDown(Key::A)) {
		position -= right * MOVE_SPEED * deltaTime;
	}
	if (m_inputManager->isKeyDown(Key::D)) {
		position += right * MOVE_SPEED * deltaTime;
	}

	transform->setPosition(position);
	transform->setRotation(rotation);
}

void PlayerControllerComponent::setInputManager(InputManager& inputManager) noexcept
{
	m_inputManager = &inputManager;
}