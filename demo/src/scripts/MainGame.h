#ifndef MAIN_GAME_SCRIPT_H
#define MAIN_GAME_SCRIPT_H
#include <script/Script.h>
#include <input/InputListener.h>

using namespace genesis;

class MainGame: public Script, InputListener
{
public:
	explicit MainGame(const ScriptDesc& desc);
	~MainGame() override;

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
	void setupScene();
	void setupMenu();

private:
	Entity* m_spaceship;
	Entity* m_asteroid;
	bool m_wasMouseLocked;
};

#endif