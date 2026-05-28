#ifndef PLAYER_MOVEMENT_H
#define PLAYER_MOVEMENT_H
#include <script/Script.h>

using namespace genesis;

class PlayerMovement final: public Script
{
public:
	explicit PlayerMovement(const ScriptDesc& desc);
	~PlayerMovement() override;

protected:
	void onAwake() override;
	void onStart() override;
	void onUpdate(float deltaTime) override;
	void onFixedUpdate(float deltaTime) override;

private:
	static const float MAX_PITCH;
	static const float MOVE_SPEED;
	static const float MOUSE_SENSITIVITY_X;
	static const float MOUSE_SENSITIVITY_Y;

private:
	TransformComponent* m_transform;
};

#endif