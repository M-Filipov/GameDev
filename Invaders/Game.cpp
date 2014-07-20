#include "Game.h"
#include "Display.h"
#include "Texture.h"
#include "Player.h"
#include "Invader.h"
#include "LTimer.h"
#include "Projectile.h"
#include "SoundEffects.h"
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>

Display disp;
Player player;
SoundEffects sound;
Texture specialTexture;
bool running;
int invaderSpeed = 1;
int rateOfFire = player.FIRE_RATE;
bool moveY = false;
int time = 0;
int remainingCols = Game::COLS;
int remainingRows = Game::ROWS;
std::vector<std::vector<Invader>> invaders;
std::vector<Projectile> projectiles;

Game::Game(void)
{
}

Game::~Game(void)
{
}

void Game::run()
{
	disp.init();
	Game::loadInvaders();

	LTimer fpsTimer;
	LTimer capTimer;
	
	int countedFrames = 0;
	fpsTimer.start();

	const int SCREEN_FPS = 60;
	const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;
	
	running  = true;

	sound.loadMedia();

	while( running )
	{
		capTimer.start();
		update();
		render();

		float avgFPS = countedFrames / ( fpsTimer.getTicks() / 1000.f );
		if( avgFPS > 2000000 )
		{
			avgFPS = 0;
		}
		++countedFrames;

		int frameTicks = capTimer.getTicks();
		if( frameTicks < SCREEN_TICK_PER_FRAME )
		{
			SDL_Delay( SCREEN_TICK_PER_FRAME - frameTicks );
		}

	}
}

void Game::loadInvaders()
{
	for(int y = 0; y < remainingCols; ++y)
	{
		std::vector<Invader> temp;
		for(int x = 0; x < remainingRows; ++x)
		{
			temp.push_back(Invader(50+y*70,70+x*60, "res/art/03.png", disp.getRenderer()));	
		}
		invaders.push_back(temp);
	}
}

void Game::update()
{
	if(invaders.size() == 0)
		return ;

	SDL_Event e;

	while( SDL_PollEvent( &e ) != 0 )
    {
        if( e.type == SDL_QUIT )
        {
            running = false;
        }

		int speed = 10;
		if( e.key.keysym.sym )
		{
			switch( e.key.keysym.sym )
			{
				case SDLK_LEFT:
					speed = -speed;
					if(player.getX() > 0 && player.getX() < disp.SCREEN_WIDTH - player.WIDTH - speed)
						player.move(speed); 
					break;
				case SDLK_RIGHT:
					speed = +speed;
					if(player.getX() + speed > 0 && player.getX() < disp.SCREEN_WIDTH - player.WIDTH)
						player.move(speed); 
					break;
				case SDLK_SPACE: 
					if(rateOfFire <= 0)
					{
						projectiles.push_back( Projectile( ( player.getX()+player.WIDTH/2-3 ), player.getY(), 1 , 1, disp.getRenderer()) );
						rateOfFire = player.FIRE_RATE;
						sound.playSound(sound.getShootWav());
					}
					break;
			}
		}	
    }

	sound.playMusic(sound.getMusic());

	time++;
	if(time == 90)
	{
		int rows = 1 + ( rand() %  remainingRows - 1  );
		int cols = 1 + ( rand() %  remainingCols - 1  );
		if(!invaders[cols][rows].dead)
		projectiles.push_back( Projectile(invaders[cols][rows].getX() + Invader::WIDTH,  invaders[cols][rows].getY() + Invader::HEIGHT, -1, -1, disp.getRenderer()) );
		time = 0;
	}


	rateOfFire --;

	updateInvaders();

	if(invaders.size() == 0)
	{
		running = false;
		return ;
	}

	for(int y = 0; y < remainingCols; ++y)
	{
		for ( int x = 0; x < remainingRows; ++x )
		{
			if(invaders[y][x].x + invaderSpeed <= 0 || invaders[y][x].x >= disp.SCREEN_WIDTH - invaders[y][x].WIDTH - invaderSpeed - 10)
			{
				moveY = true;
				invaderSpeed = -invaderSpeed;
				break;
			}
		}
	}


	for(int y = 0; y < remainingCols; ++y)
	{
		for ( int x = 0; x < remainingRows; ++x )
		{
			invaders[y][x].move(invaderSpeed);
			if(moveY)
			{
				invaders[y][x].drop(10);
			}
		}
	}

	moveY = false;
	if(projectiles.size() > 0)
	{
		for(size_t i = 0; i < projectiles.size(); ++i)
		{
			projectiles[i].update();
			if(projectiles[i].getY() <= 0)
				projectiles.erase(projectiles.begin());
		}

		for(size_t p = 0; p < projectiles.size(); ++p)
		{
			for(int y = 0; y < remainingCols; ++y)
			{
				for ( int x = 0; x < remainingRows; ++x )
				{	
					if(projectiles[p].getType() == 1 && checkCollision(y, x, p))
					{
						projectiles[p].used = true;
						invaders[y][x].dead = true;
						sound.playSound(sound.getInvaderDieWav());
					}
				}
			}
			if( checkProjPlayerCollis(p) && projectiles[p].getType() == -1)
			{
				player.RIP();
				sound.playSound(sound.getPlayerDieWav());
				running =  false;
			}
		}
	}
}

void Game::render()
{
	if(invaders.size() == 0)
	{
		running = false;
		return ;
	}

	SDL_RenderClear( disp.getRenderer() );
	
	SDL_SetRenderDrawColor( disp.getRenderer(), 0x00, 0x00, 0x00, 0x00 );

	player.render(disp.getRenderer());

	for(int y = 0; y < remainingCols; ++y)
	{
		for(int x = 0; x < remainingRows; ++x)
		{
			if(!invaders[y][x].dead)
				invaders[y][x].render(disp.getRenderer());
		}
	}

	if(projectiles.size() > 0)
	{
		for (size_t i = 0; i < projectiles.size(); ++i)
		{
			if(!projectiles[i].used)
				projectiles[i].render(disp.getRenderer());
		}
	}

	SDL_RenderPresent( disp.getRenderer() );
}

void Game::updateInvaders()
{
	int numOfDeathsStart = 0;
	int numOfDeathsEnd = 0;
	for(int i = 0; i < remainingRows; ++i)
	{
		if(invaders[0][i].dead)
		{
			numOfDeathsStart ++;
		}

		if(invaders[remainingCols-1][i].dead)
		{
			numOfDeathsEnd ++;
		}
	}
	if(numOfDeathsStart == Game::ROWS)
	{
		invaders.erase(invaders.begin());
		remainingCols --;
	}
	
	if(invaders.size() > 0)
		if(numOfDeathsEnd == Game::ROWS)
		{
			invaders.erase(invaders.end() - 1);
			remainingCols --;
		}
}

bool Game::checkCollision(int col, int row, int p)
{
	if( invaders[col][row].dead  )
		return false;
		
	if( isProjInNpcX(col, row, p) && isProjInNpcY(col, row, p) )
		return true;

	return false;
}

bool Game::isProjInNpcX (int col, int row, int p)
{
	if( projectiles[p].getX() > invaders[col][row].getX() && projectiles[p].getX() < (invaders[col][row].getX() + invaders[col][row].WIDTH) && !projectiles[p].used)
		return true;

	return false;
}

bool Game::isProjInNpcY (int col, int row, int p)
{
	if( ( projectiles[p].getY() > invaders[col][row].getY()) && projectiles[p].getY() < ( invaders[col][row].getY() + invaders[col][row].HEIGHT ) && !projectiles[p].used)
		return true;

	return false;
}

bool Game::checkProjPlayerCollis(int p)
{
	bool isProjInPlayerX = (projectiles[p].getX() > player.getX() && projectiles[p].getX() < player.getX() + player.WIDTH && !projectiles[p].used);
	bool isProjInPlayerY = (projectiles[p].getY() > player.getY() && projectiles[p].getY() < player.getY() + player.HEIGHT  && !projectiles[p].used);

	if(isProjInPlayerX && isProjInPlayerY )
		return true;

	return false;
}

void Game::startSpecialScreen(int end)
{
	running = false;
}