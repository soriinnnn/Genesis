#include "SpaceshipController.h"
#include "../utils/GameState.h"
#include "../utils/Constants.h"
#include "../utils/Macros.h"
#include "../utils/Utils.h"

#include <input/InputManager.h>
#include <entity/components/TransformComponent.h>
#include <math/Point.h>

using namespace constants;
using namespace utils;

#define NORMAL_MOVEMENT_SPEED 20.0f
#define MOUSE_SENSITIVITY_X 0.002f
#define MOUSE_SENSITIVITY_Y 0.002f
#define ORBIT_DISTANCE 30.0f
#define LERP_SPEED 5.0f

constexpr float BASE_PITCH = 0.3f;
constexpr float MAX_PITCH = 1.5f - BASE_PITCH;
constexpr float MIN_PITCH = -1.5f;

SpaceshipController::SpaceshipController(const ScriptDesc& desc): Script(desc)
{
	m_camera = nullptr;
	m_shipTransform = nullptr;
	m_movementSpeed = NORMAL_MOVEMENT_SPEED;
}

SpaceshipController::~SpaceshipController() {}

void SpaceshipController::onAwake()
{
	Entity* entity = getEntity();

	m_shipTransform = entity->getComponent<TransformComponent>();
	if (!m_shipTransform) {
		m_shipTransform = entity->createComponent<TransformComponent>();
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
	updateRotation(deltaTime);
	updatePosition(deltaTime);
	updateCamera();
}

void SpaceshipController::onFixedUpdate(float deltaTime) {}

void SpaceshipController::updateRotation(float deltaTime)
{
	Vec3 currentRotation = m_shipTransform->getRotation();
	Point delta = m_context.input.getMouseDelta();

	if (m_context.input.isMouseLocked()) {
		m_orbitRotation.x += delta.y * MOUSE_SENSITIVITY_X;
		m_orbitRotation.y += delta.x * MOUSE_SENSITIVITY_Y;
		clamp(m_orbitRotation.x, MIN_PITCH, MAX_PITCH);
		m_targetRotation = m_orbitRotation;
	}

	m_shipTransform->setRotation(Vec3::lerp(currentRotation, m_targetRotation, LERP_SPEED * deltaTime));
}

void SpaceshipController::updatePosition(float deltaTime)
{
	if (!m_context.input.isMouseLocked()) {
		return;
	}

	auto& input = m_context.input;

	Vec3 position = m_shipTransform->getPosition();
	Vec3 forward = m_shipTransform->getForwardVector();
	Vec3 right = Vec3::normalize(Vec3::cross(Vec3{0.0f, 1.0f, 0.0f}, forward));

	if (input.isKeyDown(Key::W)) {
		position += forward * m_movementSpeed * deltaTime;
	}
	if (input.isKeyDown(Key::S)) {
		position -= forward * m_movementSpeed * deltaTime;
	}
	if (input.isKeyDown(Key::D)) {
		position += right * m_movementSpeed * deltaTime;
	}
	if (input.isKeyDown(Key::A)) {
		position -= right * m_movementSpeed * deltaTime;
	}

	m_shipTransform->setPosition(position);
}

void SpaceshipController::updateCamera()
{
	TransformComponent* cameraTransform = m_camera->getComponent<TransformComponent>();
	Point delta = m_context.input.getMouseDelta();

	if (!m_context.input.isMouseLocked() && m_context.input.isMouseDown(MouseButton::Right)) {
		m_orbitRotation.x += delta.y * MOUSE_SENSITIVITY_X;
		m_orbitRotation.y += delta.x * MOUSE_SENSITIVITY_Y;
		clamp(m_orbitRotation.x, MIN_PITCH, MAX_PITCH);
	}

	float pitch = BASE_PITCH + m_orbitRotation.x;
	float yaw = m_orbitRotation.y;

	Vec3 offset = {
		-ORBIT_DISTANCE * cosf(pitch) * sinf(yaw),
		 ORBIT_DISTANCE * sinf(pitch),
		-ORBIT_DISTANCE * cosf(pitch) * cosf(yaw)
	};

	Vec3 cameraPosition = m_shipTransform->getPosition() + offset;
	cameraTransform->setPosition(cameraPosition);
	cameraTransform->setRotation({pitch, yaw, 0.0f});
}