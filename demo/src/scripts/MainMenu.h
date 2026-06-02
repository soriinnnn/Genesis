#ifndef MAIN_MENU_SCRIPT_H
#define MAIN_MENU_SCRIPT_H
#include <script/Script.h>

using namespace genesis;

class MainMenu: public Script
{
public:
	explicit MainMenu(const ScriptDesc& desc);
	~MainMenu() override;

protected:
	void onAwake() override;
	void onStart() override;
	void onUpdate(float deltaTime) override;
	void onFixedUpdate(float deltaTime) override;

private:
	void setupScene();

private:
	Entity* m_camera;
	Entity* m_spaceship;
};

#endif