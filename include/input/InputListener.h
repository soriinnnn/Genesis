#ifndef GENESIS_INPUT_LISTENER_H
#define GENESIS_INPUT_LISTENER_H
#include <input/InputCodes.h>
#include <math/Point.h>

namespace genesis
{
	class InputListener
	{
	public:
		virtual ~InputListener();

		virtual void onKeyDown(Key key) = 0;
		virtual void onKeyUp(Key key) = 0;
		virtual void onMouseMove(Point delta, Point pos) = 0;
		virtual void onMouseDown(MouseButton button, Point pos) = 0;
		virtual void onMouseUp(MouseButton button, Point pos) = 0;

	protected:
		InputListener();
	};
}

#endif