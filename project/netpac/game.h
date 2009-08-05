#ifndef GAME_H
#define GAME_H

#include "graphics.h"
#include "level.h"
#include "input.h"
#include "network.h"

class Game
{
public:
	Game();
	~Game();

	void Update();
	bool bEndGame;
	float fTickCount;
protected:
};

#endif