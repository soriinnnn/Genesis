#ifndef SPACESHIP_CONTROLLER_SCRIPT_H
#define SPACESHIP_CONTROLLER_SCRIPT_H
#include <script/Script.h>
#include <input/InputListener.h>
#include <math/Vec3.h>

using namespace genesis;

class SpaceshipController final: public Script, InputListener
{
public:
	explicit SpaceshipController(const ScriptDesc& desc);
	~SpaceshipController() override;

	void onKeyDown(Key key);
	void onKeyUp(Key key);
	void onMouseMove(Point delta, Point pos);
	void onMouseDown(MouseButton button, Point pos);
	void onMouseUp(MouseButton button, Point pos);

protected:
	void onAwake() override;
	void onStart() override;
	void onUpdate(float deltaTime) override;
	void onFixedUpdate(float deltaTime) override;

private:
	void updateRotation(float deltaTime);
	void updatePosition(float deltaTime);
	void updateCamera(float deltaTime);

private:
	Entity* m_camera;
	TransformComponent* m_transform;
	Vec3 m_targetRotation;
	Vec3 m_orbitRotation;
	float m_currentOrbitDistance;
	float m_targetOrbitDistance;
	float m_movementSpeed;
};

#endif