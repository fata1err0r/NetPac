#include "StdAfx.h"

#include "game.h"

int main( int argc, char* args[] )
{
	if(SDL_Init(0)==-1)
	{
		printf("SDL_Init: %s\n",SDL_GetError());
		exit(1);
	}

	cout<< "Welcome to NetPac 2007!\n\nCode by Dane Lipscombe (danelipscombe@gmail.com)\n\nArt by "
		<< "Neil 'of Doom' McDonald (nmcdonald@aie.edu.au)\nAnd Georgios Taraganis (gtaraganis@gmail.com)\n\n";

	Game *pGame = new Game();
	while(!pGame->bEndGame)
	{
		pGame->Update();
	}
	delete pGame;
	SDL_Quit();


	return 0;
}