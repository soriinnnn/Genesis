#include <entity/components/PlayerController.h>
#include <entity/components/Transform.h>
#include <entity/Entity.h>
#include <core/utils/Macros.h>

using namespace genesis;

const float PlayerController::MAX_PITCH = 1.553f;
const float PlayerController::MOVE_SPEED = 1.0f;

PlayerController::PlayerController(const ComponentDesc& desc): Component(desc),
	m_moveForward{false},
	m_moveBackward{false},
	m_moveLeft{false},
	m_moveRight{false}
{}

PlayerController::~PlayerController() {}

void PlayerController::update(float deltaTime)
{
	Transform* transform = m_entity.getComponent<Transform>();
	GENESIS_ASSERT(transform != nullptr, "PlayerController requires a Transform component.");

	Vec3 position = transform->getPosition();
	Vec3 rotation = transform->getRotation();
	Vec3 forward{
		 std::cos(rotation.x) * std::sin(rotation.y),
		-std::sin(rotation.x),
		 std::cos(rotation.x) * std::cos(rotation.y)
	};
	Vec3 right = Vec3::normalize(Vec3::cross(Vec3{0.0f, 1.0f, 0.0f}, forward));

	if (m_moveForward) {
		position += forward * MOVE_SPEED * deltaTime;
	}
	if (m_moveBackward) {
		position -= forward * MOVE_SPEED * deltaTime;
	}
	if (m_moveLeft) {
		position -= right * MOVE_SPEED * deltaTime;
	}
	if (m_moveRight) {
		position += right * MOVE_SPEED * deltaTime;
	}

	rotation.x += m_delta.y * deltaTime;
	rotation.y += m_delta.x * deltaTime;
	if (rotation.x > MAX_PITCH) rotation.x = MAX_PITCH;
	if (rotation.x < -MAX_PITCH) rotation.x = -MAX_PITCH;

	transform->setPosition(position);
	transform->setRotation(rotation);

	m_delta = Point{};
}

void PlayerController::onKeyDown(Key key)
{
	if (key == Key::W) {
		m_moveForward = true;
	}
	if (key == Key::S) {
		m_moveBackward = true;
	}
	if (key == Key::A) {
		m_moveLeft = true;
	}
	if (key == Key::D) {
		m_moveRight = true;
	}
}

void PlayerController::onKeyUp(Key key)
{
	if (key == Key::W) {
		m_moveForward = false;
	}
	if (key == Key::S) {
		m_moveBackward = false;
	}
	if (key == Key::A) {
		m_moveLeft = false;
	}
	if (key == Key::D) {
		m_moveRight = false;
	}
}

void PlayerController::onMouseMove(Point delta, Point pos)
{
	m_delta = delta;
}

void PlayerController::onMouseDown(MouseButton button, Point pos) {}

void PlayerController::onMouseUp(MouseButton button, Point pos) {}