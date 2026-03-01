#ifndef GENESIS_INPUT_MANAGER_H
#define GENESIS_INPUT_MANAGER_H
#include <core/Base.h>
#include <core/Core.h>
#include <input/InputListener.h>
#include <input/InputKeyCodes.h>
#include <vector>

namespace genesis
{
	struct InputManagerDesc
	{
		BaseDesc base;
	};

	class InputManager: public Base
	{
	public:
		virtual ~InputManager() override;

		virtual void update() = 0;
		void addListener(InputListener* listener);
		void removeListener(InputListener* listener);

		virtual bool isKeyDown(Key key) const = 0;
		virtual bool isKeyPressed(Key key) const = 0;
		virtual bool isKeyReleased(Key key) const = 0;

		static UniquePtr<InputManager> create(const InputManagerDesc& desc);

	protected:
		explicit InputManager(const InputManagerDesc& desc);

	protected:
		std::vector<InputListener*> m_listeners;
	};
}

#endif