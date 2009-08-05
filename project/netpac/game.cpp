#include "stdafx.h"
#include "game.h"

Game::Game()
{
	DNetwork->Init();

	DInput->Init();

	DSound->Init();

	bEndGame=false;

	DGraphics->Init();
	DLevel->Init();
}

void Game::Update()
{
	DInput->Update();
	DNetwork->Update();
	fTickCount = DGraphics->GetEngine()->AccurateTimeElapsed();
	
	if(DInput->IsKeySingle(SDLK_f))
	{
		if(DSound->SoundOn)
		{
			DSound->SoundOn=false;
			cout << "Sound Disabled\n";
		}
		else
		{
			DSound->SoundOn=true;
			cout << "Sound Enabled\n";
		}
	}
	if(DLevel->bIsServer)
	{
		if(DNetwork->iNumClients == 1)
		{
			if(DInput->IsKeySingle(SDLK_F5))
			{
				DLevel->LoadLevels();
				DLevel->StartLevel(0);
			}
		}
	}

	if(DInput->IsKey(SDLK_ESCAPE))
		bEndGame=true;

	if(DInput->WindowClosed())
		bEndGame=true;

	DGraphics->GetEngine()->Clear(false);

	DLevel->Update();

	DGraphics->GetEngine()->RenderToScreen();
}

Game::~Game()
{
	DNetwork->Shutdown();
}