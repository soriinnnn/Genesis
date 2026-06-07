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
	void updateRotation(float deltaTime);
	void updatePosition(float deltaTime);
	void updateCamera();

private:
	Entity* m_camera;
	TransformComponent* m_shipTransform;
	Vec3 m_targetRotation;
	Vec3 m_orbitRotation;
	float m_movementSpeed;
};

#endif