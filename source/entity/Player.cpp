#include <entity/Player.h>
#include <entity/components/Transform.h>
#include <entity/components/Camera.h>

using namespace genesis;

Player::Player(const EntityDesc& desc): Entity(desc) 
{
	Transform* transform = createComponent<Transform>();
	transform->setPosition(Vec3{0, 0, -1});

	Camera* camera = createComponent<Camera>();
	camera->setFov(1.57f);
}

Player::~Player() {}

void Player::update(float deltaTime) {}