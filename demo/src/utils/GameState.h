#ifndef GAME_STATE_H
#define GAME_STATE_H

enum class GameState
{
	MainMenu,
	Playing,
	Paused
};

extern GameState gameState;

#endif