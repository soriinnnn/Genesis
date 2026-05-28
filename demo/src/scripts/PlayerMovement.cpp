#include "PlayerMovement.h"
#include <entity/components/TransformComponent.h>
#include <input/InputManager.h>
#include <math/Point.h>

const float PlayerMovement::MAX_PITCH = 1.553f;
const float PlayerMovement::MOVE_SPEED = 1.0f;
const float PlayerMovement::MOUSE_SENSITIVITY_X = 0.001f;
const float PlayerMovement::MOUSE_SENSITIVITY_Y = 0.001f;

PlayerMovement::PlayerMovement(const ScriptDesc& desc): Script(desc) 
{
	m_transform = nullptr;
}

PlayerMovement::~PlayerMovement() {}

void PlayerMovement::onAwake() 
{
	m_transform = getEntity()->getComponent<TransformComponent>();
	if (!m_transform) {
		GENESIS_LOG_THROW_ERROR("Entity is missing a TransformComponent.");
	}
}

void PlayerMovement::onStart() {}

void PlayerMovement::onUpdate(float deltaTime) 
{
	auto& input = m_context.input;
	if (!input.isMouseLocked()) {
		return;
	}

	Vec3 position = m_transform->getPosition();
	Vec3 rotation = m_transform->getRotation();

	Point delta = input.getMouseDelta();
	rotation.x += delta.y * MOUSE_SENSITIVITY_X;
	rotation.y += delta.x * MOUSE_SENSITIVITY_Y;
	if (rotation.x > MAX_PITCH) rotation.x = MAX_PITCH;
	if (rotation.x < -MAX_PITCH) rotation.x = -MAX_PITCH;

	Vec3 forward = m_transform->getForwardVector();
	Vec3 right = Vec3::normalize(Vec3::cross(Vec3{0.0f, 1.0f, 0.0f}, forward));

	if (input.isKeyDown(Key::W)) {
		position += forward * MOVE_SPEED * deltaTime;
	}
	if (input.isKeyDown(Key::S)) {
		position -= forward * MOVE_SPEED * deltaTime;
	}
	if (input.isKeyDown(Key::A)) {
		position -= right * MOVE_SPEED * deltaTime;
	}
	if (input.isKeyDown(Key::D)) {
		position += right * MOVE_SPEED * deltaTime;
	}

	m_transform->setPosition(position);
	m_transform->setRotation(rotation);
}

void PlayerMovement::onFixedUpdate(float deltaTime) {}