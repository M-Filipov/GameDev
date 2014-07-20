#pragma once
#include <SDL.h>
#include <string>
#include <SDL_image.h>

class Texture
{
public:
	Texture(void);

	bool LoadFromFile(std::string path, SDL_Renderer* gRenderer);

	void free();

	void render( int x, int y, SDL_Renderer* gRenderer, SDL_Rect* clip = NULL );

	int getWidth();

	int getHeight();

	SDL_Texture* getTexture();

	~Texture(void);

private:
	SDL_Texture* mTexture;

	int mHeight;
	int mWidth;
};

