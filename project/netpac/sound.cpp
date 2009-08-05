#include "stdafx.h"

void Sound::Init()
{
	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
	{
		cout << "Error Initialising Audio Device!\n";
	}

	music = Mix_LoadMUS("Media/fight.xm");
	music2 = Mix_LoadMUS("Media/woohoo.xm");
	waka = Mix_LoadWAV("Media/waka.wav");
	pill = Mix_LoadWAV("Media/pill.wav");
	die = Mix_LoadWAV("Media/die.wav");
	SoundOn=true;
}

void Sound::PlayWaka()
{
	if (SoundOn)
			Mix_PlayChannel( 1, waka, 0 );
}

void Sound::PlayPill()
{
	if (SoundOn)
		Mix_PlayChannel( 2, pill, 0 );
}

void Sound::PlayDie()
{
	if (SoundOn)
		Mix_PlayChannel( 3, die, 0 );
}

void Sound::PlayWoohoo()
{
	if (SoundOn)
		Mix_PlayMusic( music2, 0 );
}

void Sound::PlayFight()
{
	if (SoundOn)
		Mix_PlayMusic( music, 0 );
}