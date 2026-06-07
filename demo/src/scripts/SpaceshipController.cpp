#include "SpaceshipController.h"
#include "../utils/GameState.h"
#include "../utils/Constants.h"
#include "../utils/Macros.h"

#include <input/InputManager.h>
#include <entity/components/TransformComponent.h>
#include <math/Point.h>

using namespace constants;

#define MOVE_SPEED 1.0f

#define MAX_PITCH 1.4f
#define MOUSE_SENSITIVITY_X 0.002f
#define MOUSE_SENSITIVITY_Y 0.002f

#define MAX_BANK_ANGLE 0.5f
#define BANK_MULTIPLIER 4.0f
#define BANK_DECAY 10.0f
#define LERP_SPEED 6.5f

SpaceshipController::SpaceshipController(const ScriptDesc& desc): Script(desc)
{
	m_camera = nullptr;
	m_transform = nullptr;
	m_pitch = 0.0f;
	m_yaw = 0.0f;
	m_bank = 0.0f;
}

SpaceshipController::~SpaceshipController() {}

void SpaceshipController::onAwake()
{
	Entity* entity = getEntity();

	m_transform = entity->getComponent<TransformComponent>();
	if (!m_transform) {
		m_transform = entity->createComponent<TransformComponent>();
	}
}

void SpaceshipController::onStart() 
{
	m_camera = m_context.entities.getEntityByName(ENTITIES_MAIN_CAMERA);
	if (!m_camera) {
		GENESIS_LOG_THROW_ERROR("Entity \"{}\" not found.", ENTITIES_MAIN_CAMERA);
	}
	updateCamera();
}

void SpaceshipController::onUpdate(float deltaTime)
{
	if (gameState != GameState::Playing) {
		return;
	}
	updateSpaceshipRot(deltaTime);
	updateSpaceshipPos(deltaTime);
	updateCamera();
}

void SpaceshipController::onFixedUpdate(float deltaTime) {}

void SpaceshipController::updateSpaceshipRot(float deltaTime)
{
	if (!m_context.input.isMouseLocked()) {
		return;
	}

	Point delta = m_context.input.getMouseDelta();
	
	m_pitch += delta.y * MOUSE_SENSITIVITY_X;
	m_yaw += delta.x * MOUSE_SENSITIVITY_Y;
	if (m_pitch > MAX_PITCH) m_pitch = MAX_PITCH;
	if (m_pitch < -MAX_PITCH) m_pitch = -MAX_PITCH;

	float targetBank = -delta.x * MOUSE_SENSITIVITY_Y * BANK_MULTIPLIER;
	if (targetBank > MAX_BANK_ANGLE) targetBank = MAX_BANK_ANGLE;
	if (targetBank < -MAX_BANK_ANGLE) targetBank = -MAX_BANK_ANGLE;
	m_bank = std::lerp(m_bank, targetBank, BANK_DECAY * deltaTime);

	Vec3 currentRotation = m_transform->getRotation();
	Vec3 targetRotation = {m_pitch, m_yaw, m_bank};
	m_transform->setRotation(Vec3::lerp(currentRotation, targetRotation, LERP_SPEED * deltaTime));
}

void SpaceshipController::updateSpaceshipPos(float deltaTime)
{
	if (!m_context.input.isMouseLocked()) {
		return;
	}

	auto& input = m_context.input;

	Vec3 position = m_transform->getPosition();
	Vec3 forward = m_transform->getForwardVector();
	Vec3 right = Vec3::normalize(Vec3::cross(Vec3{0.0f, 1.0f, 0.0f}, forward));

	if (input.isKeyDown(Key::W)) {
		position += forward * MOVE_SPEED * deltaTime;
	}
	if (input.isKeyDown(Key::S)) {
		position -= forward * MOVE_SPEED * deltaTime;
	}
	if (input.isKeyDown(Key::D)) {
		position += right * MOVE_SPEED * deltaTime;
	}
	if (input.isKeyDown(Key::A)) {
		position -= right * MOVE_SPEED * deltaTime;
	}

	GENESIS_LOG_INFO("POS: X={}, Y={}, Z={}", position.x, position.y, position.z);

	m_transform->setPosition(position);
}

void SpaceshipController::updateCamera()
{
	TransformComponent* cameraTransform = m_camera->getComponent<TransformComponent>();

	Vec3 forward = m_transform->getForwardVector();
	Vec3 right = m_transform->getRightVector();
	Vec3 up = m_transform->getUpVector();

	Vec3 camPosition = m_transform->getPosition() +
		forward * cameraPlayerOffset.z +
		right * cameraPlayerOffset.x +
		up * cameraPlayerOffset.y;

	Vec3 dirToPlayer = Vec3::normalize(m_transform->getPosition() - camPosition);
	
	float pitch = asin(-dirToPlayer.y);
	float yaw = atan2(dirToPlayer.x, dirToPlayer.z);

	cameraTransform->setPosition(camPosition);
	cameraTransform->setRotation({pitch, yaw, 0.0f});
}