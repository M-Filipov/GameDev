#pragma once
#include <SDL.h>
#include <string>
#include <SDL_image.h>
#include <SDL_mixer.h>


class Display
{
public:
	Display(void);

	~Display(void);

	bool init();

	bool loadMedia();

	SDL_Renderer* getRenderer();

	SDL_Window* getWindow();

	static const int SCREEN_WIDTH = 1024;
	
	static const int SCREEN_HEIGHT = 640;

private:


	SDL_Window* window;

	SDL_Renderer* renderer;

};

