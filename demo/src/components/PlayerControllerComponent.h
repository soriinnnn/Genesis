#ifndef PLAYER_CONTROLLER_COMPONENT_H
#define PLAYER_CONTROLLER_COMPONENT_H
#include <entity/components/Component.h>

namespace genesis
{
	class PlayerControllerComponent: public Component
	{
	public:
		explicit PlayerControllerComponent(const ComponentDesc& desc);
		~PlayerControllerComponent() override;

		void update(float deltaTime) override;
		void setInputManager(InputManager& inputManager) noexcept;

	private:
		static const float MAX_PITCH;
		static const float MOVE_SPEED;
		static const float MOUSE_SENSITIVITY_X;
		static const float MOUSE_SENSITIVITY_Y;

	private:
		InputManager* m_inputManager;
	};
}

#endif