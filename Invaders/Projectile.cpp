#include "Projectile.h"
#include "Texture.h"

Texture ptexture;


Projectile::Projectile(int  x, int y, int dir, int type, SDL_Renderer* gRenderer)
{
	Projectile::x = x;
	Projectile::y = y;
	Projectile::dir = dir;
	Projectile::used = false;
	Projectile::type = type;

	ptexture.LoadFromFile("res/art/projectile.png", gRenderer);
	projectileTexture = ptexture.getTexture();

}


Projectile::~Projectile(void)
{
}

void Projectile::update()
{
	move();
}

void Projectile::render(SDL_Renderer* gRenderer)
{
	SDL_Rect projectileRect = { x-200, y, WIDTH, HEIGHT };

	ptexture.render(x,y,gRenderer,&projectileRect);
}

void Projectile::move()
{
	Projectile::y -= SPEED * dir;
}

int Projectile::getX()
{
	return Projectile::x;
}

int Projectile::getY()
{
	return Projectile::y;
}

int Projectile::getType()
{
	return Projectile::type;
}