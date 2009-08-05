#ifndef SOUND_H
#define SOUND_H

#include "singleton.h"
#include "SDL/SDL_mixer.h"

class Sound : public Singleton<Sound>
{
	friend Singleton;
public:
	void Init();
	void PlayWaka();
	void PlayPill();
	void PlayDie();
	void PlayWoohoo();
	void PlayFight();
	bool SoundOn;
private:
	Mix_Music *music, *music2;
	Mix_Chunk *waka, *pill, *die;
protected:
	Sound(){};
};

#define DSound Sound::Instance()
#endif