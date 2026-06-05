#ifndef MAIN_MENU_SCRIPT_H
#define MAIN_MENU_SCRIPT_H
#include "../utils/Types.h"
#include <script/Script.h>

using namespace genesis;
using namespace types;

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
	void setupMenu();

private:
	Entity* m_spaceship;
	Entity* m_asteroid1;
	Entity* m_asteroid2;
	Entity* m_asteroid3;
	Animation m_spaceshipPosAnim;
	Animation m_spaceshipRotAnim;
	Animation m_asteroid1PosAnim;
	Animation m_asteroid1RotAnim;
	Animation m_asteroid2PosAnim;
	Animation m_asteroid2RotAnim;
	Animation m_asteroid3RotAnim;
};

#endif