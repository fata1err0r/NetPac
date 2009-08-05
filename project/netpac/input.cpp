#include "input.h"
#include "stdafx.h"

// This class controls game input.

void Input::Init()
{
	keys = SDL_GetKeyState(NULL);
	//SDL_WM_GrabInput(SDL_GRAB_ON);
	//SDL_ShowCursor(0);
	SDL_PollEvent(&SDLEvent);
	SDL_WarpMouse(320, 240);
}


//returns true if a key is currently pressed.
bool Input::IsKey(SDLKey key)
{
	if (keys[key])
		return true;

	return false;
}

bool Input::IsKeySingle(int i)
{
	if (keys[i] && !lastkeys[i])
		return true;
	
	return false;
}

//returns true if user closed window
bool Input::WindowClosed()
{
	if(SDLEvent.type == SDL_QUIT)
		return true;
	return false;
}

void Input::Update()
{
	//store previously pressed keys
	for (int i = 0; i < SDLK_LAST; i++){
		lastkeys[i] = keys[i];
	}

	SDL_PollEvent(&SDLEvent);

	switch(SDLEvent.type)
	{
	case SDL_MOUSEMOTION:
		//get the offset and lock the mouse to the centre of the screen
		MouseOffset.x = 320-SDLEvent.button.x;
		MouseOffset.y = 240-SDLEvent.button.y;
		//SDL_WarpMouse(320, 240);
	}
	
}

//returns relative mouse offset
cTV_2DVECTOR Input::GetMouseOffset()
{
	return MouseOffset;

}