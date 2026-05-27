#ifndef GENESIS_INPUT_LISTENER_H
#define GENESIS_INPUT_LISTENER_H
#include <core/Core.h>
#include <input/InputCodes.h>
#include <math/Point.h>

GENESIS_NAMESPACE_BEGIN

class InputListener
{
public:
	virtual ~InputListener() = default;

	virtual void onKeyDown(Key key) = 0;
	virtual void onKeyUp(Key key) = 0;
	virtual void onMouseDown(MouseButton button, Point pos) = 0;
	virtual void onMouseUp(MouseButton button, Point pos) = 0;
	virtual void onMouseMove(Point delta, Point pos) = 0;
};

GENESIS_NAMESPACE_END

#endif