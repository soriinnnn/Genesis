#ifndef GENESIS_PLAYER_CONTROLLER_H
#define GENESIS_PLAYER_CONTROLLER_H
#include <entity/components/Component.h>

namespace genesis
{
	class PlayerController: public Component
	{
	public:
		explicit PlayerController(const ComponentDesc& desc);
		~PlayerController() override;

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