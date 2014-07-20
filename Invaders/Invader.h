#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>


class Invader
{
public:
	Invader(int x, int y, std::string textureName, SDL_Renderer* gRenderer);


	~Invader(void);

	void move(int direction);

	void render(SDL_Renderer* gRendere);

	void shoot();

	void drop(int distance);

	int getX();

	int getY();

	int x;

	int y;
	
	static const int WIDTH = 40;

	static const int HEIGHT = 25;

	bool dead;

	SDL_Texture* getTexture();

	static bool Invader::turn;

private:	

	SDL_Texture* invaderTexture;

	std::string path;

};

