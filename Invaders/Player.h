#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

class Player
{
public:
	Player(void);

	~Player(void);

	void move(int direction);

	void render(SDL_Renderer* gRenderer);

	int getX();

	int getY();

	bool isDead();

	void RIP();

	static const int WIDTH = 60;

	static const int HEIGHT = 35;

	static const int FIRE_RATE = 20;

private:	

	int x;
	int y;

	bool dead;

	SDL_Texture* playerTexture;
};