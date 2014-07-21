#include "SoundEffects.h"
#include <stdio.h>

SoundEffects::SoundEffects(void)
{
}


SoundEffects::~SoundEffects(void)
{
}

bool SoundEffects::loadMedia()
{
	shootWav = Mix_LoadWAV("res/sound/ShipBullet.wav");
	if (shootWav == NULL)
	{
		printf("Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		return false;
	}

	invaderDieWav = Mix_LoadWAV("res/sound/InvaderKilled.wav");
	if (invaderDieWav == NULL)
	{
		printf("Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		return false;
	}

	playerDieWav = Mix_LoadWAV("res/sound/InvaderHit.wav");
	if (playerDieWav == NULL)
	{
		printf("Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		return false;
	}

	music = Mix_LoadMUS("res/sound/03.wav");
	if (music == NULL)
	{
		printf("Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		return false;
	}

	return true;
}

void SoundEffects::playSound(Mix_Chunk* sound)
{
	Mix_PlayChannel(-1, sound, 0);
}

void SoundEffects::playMusic(Mix_Music* music)
{
	if( Mix_PlayingMusic() == 0 )
    {
        Mix_PlayMusic( music, -1 );
    }
}

Mix_Chunk *SoundEffects::getShootWav()
{
	return SoundEffects::shootWav;
}

Mix_Chunk* SoundEffects::getInvaderDieWav()
{
	return SoundEffects::invaderDieWav;
}

Mix_Chunk* SoundEffects::getPlayerDieWav()
{
	return SoundEffects::playerDieWav;
}

Mix_Music* SoundEffects::getMusic()
{
	return SoundEffects::music;
}

