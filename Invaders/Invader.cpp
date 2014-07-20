#include "Invader.h"
#include "Texture.h"
#include "Display.h"

Texture gtexture;

bool Invader::turn;

Invader::Invader(int x, int y, std::string textureName, SDL_Renderer* gRenderer)
{
	Invader::x = x;
	Invader::y = y;
	Invader::dead = false;
	Invader::path = textureName;

	gtexture.LoadFromFile(path.c_str(), gRenderer);
	invaderTexture = gtexture.getTexture();
}


Invader::~Invader(void)
{
}

void Invader::move(int direction)
{	
	x += direction;
}

void Invader::drop(int distance)
{	
	y += distance;
}

void Invader::render(SDL_Renderer* gRenderer)
{
	SDL_Rect invaderRect = { x, y, WIDTH, HEIGHT };

	gtexture.render(x,y,gRenderer,&invaderRect);
}

int Invader::getX()
{
	return Invader::x;
}

int Invader::getY()
{
	return Invader::y;
}

