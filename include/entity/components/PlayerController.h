#ifndef GENESIS_PLAYER_CONTROLLER_H
#define GENESIS_PLAYER_CONTROLLER_H
#include <entity/components/Component.h>
#include <input/InputListener.h>

namespace genesis
{
	class PlayerController: public Component, public InputListener
	{
	public:
		explicit PlayerController(const ComponentDesc& desc);
		~PlayerController() override;

		void update(float deltaTime);

		void onKeyDown(Key key) override;
		void onKeyUp(Key key) override;
		void onMouseMove(Point delta, Point pos) override;
		void onMouseDown(MouseButton button, Point pos) override;
		void onMouseUp(MouseButton button, Point pos) override;

	private:
		static const float MAX_PITCH;
		static const float MOVE_SPEED;

		bool m_moveForward{false};
		bool m_moveBackward{false};
		bool m_moveLeft{false};
		bool m_moveRight{false};
		Point m_delta;
	};
}

#endif