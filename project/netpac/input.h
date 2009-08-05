#ifndef INPUT_H
#define INPUT_H

#include "singleton.h"

class Input : public Singleton<Input>
{
	friend Singleton;
public:
	bool IsKey(SDLKey key);
	bool WindowClosed();
	cTV_2DVECTOR GetMouseOffset();
	void Init();
	void Update();
	bool IsKeySingle(int i);
private:
protected:
	Input(){};
	SDL_Event SDLEvent;
	Uint8 *keys;
	Uint8 lastkeys[SDLK_LAST];
	cTV_2DVECTOR MouseOffset;
};

#define DInput Input::Instance()

#endif