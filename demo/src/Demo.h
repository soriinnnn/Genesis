#ifndef DEMO_H
#define DEMO_H
#include <game/Game.h>
#include <ui/elements/UILabel.h>

using namespace genesis;

class Demo final: public Game, public InputListener
{
public:
	explicit Demo(const GameDesc& desc);
	~Demo() override;

	void onKeyDown(Key key);
	void onKeyUp(Key key);
	void onMouseMove(Point delta, Point pos);
	void onMouseDown(MouseButton button, Point pos);
	void onMouseUp(MouseButton button, Point pos);

protected:
	void onCreate() override;
	void onUpdate(float deltaTime) override;

private:
	bool m_centerMouse;
};

#endif