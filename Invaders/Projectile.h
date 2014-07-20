#pragma once
#include <SDL.h>
#include <SDL_image.h>

class Projectile
{
public:
	Projectile(int x, int y, int dir, int type, SDL_Renderer* gRenderer);
	
	~Projectile(void);

	void update();

	void render(SDL_Renderer* gRenderer);

	int getX();

	int getY();

	int getType();

	bool used;

	static const int WIDTH = 7;

	static const int HEIGHT = 18;

	static const int SPEED = 12;

private:

	void move();

	SDL_Texture* projectileTexture;

	int x;

	int y;

	int dir;

	int type;
	
};

