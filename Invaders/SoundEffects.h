#pragma once

#include <SDL_mixer.h>
#include <SDL.h>

class SoundEffects
{
public:
	SoundEffects(void);
	~SoundEffects(void);

	bool loadMedia();

	void playSound(Mix_Chunk* sound);

	void playMusic(Mix_Music* music);

	Mix_Chunk* getShootWav();
	Mix_Chunk* getInvaderDieWav();
	Mix_Chunk* getPlayerDieWav();

	Mix_Music* getMusic();

private:

	Mix_Chunk* shootWav;
	Mix_Chunk* invaderDieWav;
	Mix_Chunk* playerDieWav;

	Mix_Music* music;

};

