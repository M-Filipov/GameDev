#include "Player.h"
#include "Texture.h"
#include <stdio.h>

Texture texture;

Player::Player(void)
{
	x = 512;
	y = 600;
}


Player::~Player(void)
{
}

void Player::move(int direction)
{
	x += direction;
}

void Player::render(SDL_Renderer* gRenderer)
{
	texture.LoadFromFile("res/art/Player.png", gRenderer);
	playerTexture = texture.getTexture();
	SDL_Rect playerRect = { x-200, y, WIDTH, HEIGHT };

	texture.render(x,y,gRenderer,&playerRect);

}

int Player::getX()
{
	return Player::x;
}

int Player::getY()
{
	return Player::y;
}

bool Player::isDead()
{
	return Player::dead;
}

void Player::RIP()
{
	Player::dead = true;
}


