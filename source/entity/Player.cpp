#include <entity/Player.h>
#include <entity/components/Transform.h>

using namespace genesis;

Player::Player(const EntityDesc& desc): Entity(desc) {}

Player::~Player() {}

void Player::update(float deltaTime) {}