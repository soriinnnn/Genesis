#include "SpaceshipController.h"
#include "../utils/GameState.h"
#include "../utils/Constants.h"
#include "../utils/Macros.h"

#include <input/InputManager.h>
#include <entity/components/TransformComponent.h>
#include <math/Point.h>

using namespace constants;

#define MOVE_SPEED 1.0f

#define MAX_PITCH 1.40f
#define MOUSE_SENSITIVITY_X 0.002f
#define MOUSE_SENSITIVITY_Y 0.002f
#define BANK_MULTIPLIER 2.0f
#define MAX_BANK_ANGLE 0.1f
#define BANK_SMOOTH_SPEED 12.0f
#define LERP_SPEED 6.5f

SpaceshipController::SpaceshipController(const ScriptDesc& desc): Script(desc)
{
	m_camera = nullptr;
	m_playerTransform = nullptr;
}

SpaceshipController::~SpaceshipController() {}

void SpaceshipController::onAwake()
{
	Entity* player = getEntity();

	m_playerTransform = player->getComponent<TransformComponent>();
	if (!m_playerTransform) {
		m_playerTransform = player->createComponent<TransformComponent>();
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
	if (!m_context.input.isMouseLocked()) {
		return;
	}
	updateRotation(deltaTime);
	updatePosition(deltaTime);
	updateCamera();
}

void SpaceshipController::onFixedUpdate(float deltaTime) {}

void SpaceshipController::updateRotation(float deltaTime)
{
	Vec3 currentRotation = m_playerTransform->getRotation();
	Point delta = m_context.input.getMouseDelta();
	
	m_targetRotation.x -= delta.y * MOUSE_SENSITIVITY_X;
	m_targetRotation.y -= delta.x * MOUSE_SENSITIVITY_Y;
	if (m_targetRotation.x > MAX_PITCH) m_targetRotation.x = MAX_PITCH;
	if (m_targetRotation.x < -MAX_PITCH) m_targetRotation.x = -MAX_PITCH;

	float bankAngle = -delta.x * MOUSE_SENSITIVITY_Y * BANK_MULTIPLIER;
	if (bankAngle > MAX_BANK_ANGLE) bankAngle = MAX_BANK_ANGLE;
	if (bankAngle < -MAX_BANK_ANGLE) bankAngle = -MAX_BANK_ANGLE;
	m_targetRotation.z = bankAngle;

	Vec3 rotation = Vec3::lerp(currentRotation, m_targetRotation, LERP_SPEED * deltaTime);
	m_playerTransform->setRotation(rotation);
}

void SpaceshipController::updatePosition(float deltaTime)
{
	auto& input = m_context.input;

	Vec3 position = m_playerTransform->getPosition();
	Vec3 forward = m_playerTransform->getForwardVector();
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

	m_playerTransform->setPosition(position);
}

void SpaceshipController::updateCamera()
{
	TransformComponent* cameraTransform = m_camera->getComponent<TransformComponent>();

	Vec3 forward = m_playerTransform->getForwardVector();
	Vec3 right = m_playerTransform->getRightVector();
	Vec3 up = m_playerTransform->getUpVector();

	Vec3 camPosition = m_playerTransform->getPosition() +
		forward * cameraPlayerOffset.z +
		right * cameraPlayerOffset.x +
		up * cameraPlayerOffset.y;

	Vec3 dirToPlayer = Vec3::normalize(m_playerTransform->getPosition() - camPosition);
	
	float pitch = asin(-dirToPlayer.y);
	float yaw = atan2(dirToPlayer.x, dirToPlayer.z);

	cameraTransform->setPosition(camPosition);
	cameraTransform->setRotation({pitch, yaw, 0.0f});
}