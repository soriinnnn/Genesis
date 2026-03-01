#ifndef GENESIS_INPUT_LISTENER_H
#define GENESIS_INPUT_LISTENER_H
#include <input/InputKeyCodes.h>

namespace genesis
{
	class InputListener
	{
	public:
		virtual ~InputListener();

		virtual void onKeyDown(Key key) = 0;
		virtual void onKeyUp(Key key) = 0;

	protected:
		InputListener();
	};
}

#endif