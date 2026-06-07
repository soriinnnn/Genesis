#ifndef SPACESHIP_CONTROLLER_SCRIPT_H
#define SPACESHIP_CONTROLLER_SCRIPT_H
#include <script/Script.h>
#include <math/Vec3.h>

using namespace genesis;

class SpaceshipController final: public Script
{
public:
	explicit SpaceshipController(const ScriptDesc& desc);
	~SpaceshipController() override;

protected:
	void onAwake() override;
	void onStart() override;
	void onUpdate(float deltaTime) override;
	void onFixedUpdate(float deltaTime) override;

private:
	void updateSpaceshipRot(float deltaTime);
	void updateSpaceshipPos(float deltaTime);
	void updateCamera();

private:
	Entity* m_camera;
	TransformComponent* m_transform;
	Vec3 m_targetRotation;
	float m_pitch;
	float m_yaw;
	float m_bank;
};

#endif