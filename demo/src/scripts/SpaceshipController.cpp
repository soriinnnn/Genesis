#include "SpaceshipController.h"
#include "../utils/GameState.h"
#include "../utils/Constants.h"
#include "../utils/Macros.h"
#include "../utils/Utils.h"

#include <input/InputManager.h>
#include <resources/PostProcess.h>
#include <entity/components/TransformComponent.h>
#include <math/Point.h>

using namespace constants;
using namespace utils;

#define NORMAL_ORBIT_DISTANCE 30.0f
#define SPRINT_ORBIT_DISTANCE 40.0f
#define NORMAL_MOVEMENT_SPEED 40.0f
#define SPRINT_MOVEMENT_SPEED 80.0f;
#define MOUSE_SENSITIVITY_X 0.002f
#define MOUSE_SENSITIVITY_Y 0.002f
#define LERP_SPEED 5.0f

constexpr float BASE_PITCH = 0.18f;
constexpr float MAX_PITCH = 1.5f - BASE_PITCH;
constexpr float MIN_PITCH = -1.5f;

SpaceshipController::SpaceshipController(const ScriptDesc& desc): Script(desc)
{
	m_camera = nullptr;
	m_transform = nullptr;
	m_movementSpeed = NORMAL_MOVEMENT_SPEED;
	m_targetOrbitDistance = NORMAL_ORBIT_DISTANCE;
	m_currentOrbitDistance = m_targetOrbitDistance;
}

SpaceshipController::~SpaceshipController() 
{
	m_context.input.removeListener(this);
}

void SpaceshipController::onKeyDown(Key key) 
{
	if (key != Key::LeftShift) {
		return;
	}
	m_movementSpeed = SPRINT_MOVEMENT_SPEED;
	m_targetOrbitDistance = SPRINT_ORBIT_DISTANCE;
}

void SpaceshipController::onKeyUp(Key key) 
{
	if (key != Key::LeftShift) {
		return;
	}
	m_movementSpeed = NORMAL_MOVEMENT_SPEED;
	m_targetOrbitDistance = NORMAL_ORBIT_DISTANCE;
}

void SpaceshipController::onMouseMove(Point delta, Point pos) {}

void SpaceshipController::onMouseDown(MouseButton button, Point pos) {}

void SpaceshipController::onMouseUp(MouseButton button, Point pos) {}

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
	m_context.input.addListener(this);
}

void SpaceshipController::onUpdate(float deltaTime)
{
	if (gameState != GameState::Playing) {
		return;
	}
	updateRotation(deltaTime);
	updatePosition(deltaTime);
	updateCamera(deltaTime);
}

void SpaceshipController::onFixedUpdate(float deltaTime) {}

void SpaceshipController::updateRotation(float deltaTime)
{
	Vec3 currentRotation = m_transform->getRotation();
	Point delta = m_context.input.getMouseDelta();

	if (m_context.input.isMouseLocked()) {
		m_orbitRotation.x += delta.y * MOUSE_SENSITIVITY_X;
		m_orbitRotation.y += delta.x * MOUSE_SENSITIVITY_Y;
		clamp(m_orbitRotation.x, MIN_PITCH, MAX_PITCH);
		m_targetRotation = m_orbitRotation;
	}

	m_transform->setRotation(Vec3::lerp(currentRotation, m_targetRotation, LERP_SPEED * deltaTime));
}

void SpaceshipController::updatePosition(float deltaTime)
{
	if (!m_context.input.isMouseLocked()) {
		return;
	}

	auto& input = m_context.input;

	Vec3 position = m_transform->getPosition();
	Vec3 forward = m_transform->getForwardVector();
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

	m_transform->setPosition(position);
}

void SpaceshipController::updateCamera(float deltaTime)
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

	m_currentOrbitDistance = std::lerp(m_currentOrbitDistance, m_targetOrbitDistance, LERP_SPEED * deltaTime);
	Vec3 offset = {
		-m_currentOrbitDistance * cosf(pitch) * sinf(yaw),
		 m_currentOrbitDistance* sinf(pitch),
		-m_currentOrbitDistance * cosf(pitch) * cosf(yaw)
	};

	Vec3 cameraPosition = m_transform->getPosition() + offset;
	cameraTransform->setPosition(cameraPosition);
	cameraTransform->setRotation({pitch, yaw, 0.0f});
}