#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int secret_digits_count = 4;

int cows = 0;
int bulls = 0;

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

SDL_Texture* gTextureCow = NULL;
SDL_Texture* gTextureBull = NULL;
SDL_Texture* gTextureNums[11];
SDL_Texture* currentNumTexture = NULL;

bool hasDupes ( int n ) { 
	    int used[secret_digits_count];

	   	for (int i = 0; i < secret_digits_count; i++)
	   	{
	   		used[i] = n % 10;
	   		n /= 10;
	   	}

		for(int i = 0; i < secret_digits_count; i++) {
			for(int j = i + 1; j < secret_digits_count; j++) {
				if(used[i] == used[j])
					return true;
			}
		}

		return false;
	}

	int genNum()
	{
		int min,
			max,
			output;

		min = 1023;
		max = 9876;
		output = 0;

		
		while(hasDupes(output))
		{
			srand(time(NULL));
			output = min + ( rand() % ( max - min + 1 ) );
		}

		return output;

	}


	void clear()
	{
		cows = 0;
		bulls = 0;
		SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderClear( gRenderer );
	}

	void compare(int firstNum, int input)
	{
		int firstNumArr[secret_digits_count],
			inputArr[secret_digits_count];

		cows = 0;
		bulls = 0;
			
		for (int i = 0; i < secret_digits_count; i++)
		{
			firstNumArr[i] = firstNum % 10;
			inputArr[i] = input % 10;
			firstNum /= 10;
			input /= 10;
		}

		for (int i = 0; i < secret_digits_count; i++)
		{
			for (int j = 0; j < secret_digits_count; j++)
			{
				if (i == j && firstNumArr[i] == inputArr[j])
					bulls++;
				else if(firstNumArr[i] == inputArr[j])
					cows ++;
			}
		}
	}

bool init()
{
	bool success = true;

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		gWindow = SDL_CreateWindow( "BULLS AND COWS", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}


void close()
{
	SDL_DestroyTexture( gTextureCow );
	SDL_DestroyTexture( gTextureBull);
	gTextureCow = NULL;
	gTextureBull = NULL;

	for( int i = 0; i < 11; i++)
	{
		SDL_DestroyTexture( gTextureNums[i] );
		gTextureNums[i] = NULL;

	}

	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture( std::string path )
{
	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		SDL_FreeSurface( loadedSurface );
	}

	return newTexture;
}

bool loadMedia()
{
	bool success = true;

	gTextureCow = loadTexture( "res/cow.png" );
	if( gTextureCow == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}

	gTextureBull = loadTexture( "res/bull.png" );
	if( gTextureBull == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}

	
	for (int i = 0; i < 10; i++)
	{
		char *str = new char[9];
		char *str2 = new char[1];
		strcpy(str, "res/");
		sprintf(str2, "%d", i);
		strcat(str, str2);
		strcat(str,".png");

		gTextureNums[i] = loadTexture( str );
		if( gTextureNums[i] == NULL )
		{
			printf( "Failed to load texture image!\n" );
			success = false;
		}

	}

	gTextureNums[10] = loadTexture("res/10.png");


	return success;
}

int main( int argc, char* args[] )
{
	bool win = false;
	int hiddenNumber = genNum();

	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	

			bool quit = false;

			SDL_Event e;

			int inputNums[secret_digits_count];

			for(int i = 0; i < secret_digits_count; ++i)
				inputNums[i] = 10;

			int select = 0;

				SDL_Rect cowsViewport[secret_digits_count];
				SDL_Rect bullsViewport[secret_digits_count];
				SDL_Rect numsViewport[secret_digits_count];

			while( !quit )
			{
				while( SDL_PollEvent( &e ) != 0 )
				{
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					else if (e.type == SDL_KEYDOWN)
					{
						switch( e.key.keysym.sym)
						{
							if (select < 5)
							{
								if(win == false)
									{
									case SDLK_0:
										inputNums[select] = 0;
										currentNumTexture = gTextureNums[0];
										select ++;
										break;
									case SDLK_1:
										inputNums[select] = 1;
										currentNumTexture = gTextureNums[1];
										select ++;
										break;
									case SDLK_2:
										inputNums[select] = 2;
										currentNumTexture = gTextureNums[2];
										select ++;
										break;
									case SDLK_3:
										inputNums[select] = 3;
										currentNumTexture = gTextureNums[3];
										select ++;
										break;
									case SDLK_4:
										inputNums[select] = 4;
										currentNumTexture = gTextureNums[4];
										select ++;
										break;
									case SDLK_5:
										inputNums[select] = 5;
										currentNumTexture = gTextureNums[5];
										select ++;
										break;
									case SDLK_6:
										inputNums[select] = 6;
										currentNumTexture = gTextureNums[6];
										select ++;
										break;
									case SDLK_7:
										inputNums[select] = 7;
										currentNumTexture = gTextureNums[7];
										select ++;
										break;
									case SDLK_8:
										inputNums[select] = 8;
										currentNumTexture = gTextureNums[8];
										select ++;
										break;
									case SDLK_9:
										inputNums[select] = 9;
										currentNumTexture = gTextureNums[9];
										select ++;
										break;
								}	
								case SDLK_RETURN:

										clear();
										for( int i = 0; i < secret_digits_count; i++)
										{
											inputNums[i] = 10;								
										}
										select = 0;
										break;
							}				
							case SDLK_ESCAPE:
								close();
								return 0;	
						}
					}
				}
				if( select >= 4)
				{
					int decimalGuess = inputNums[0]*1000 + inputNums[1]*100 + inputNums[2]*10 + inputNums[3];

					if ( !hasDupes(decimalGuess) && decimalGuess > 1000)
					{
						compare(hiddenNumber, decimalGuess);
						if( bulls == 4)
							win = true;
					}
					else
					{
							clear();
							for( int i = 0; i < secret_digits_count; i++)
							{
								inputNums[i] = 10;								
							}
							select = 0;						
					}
				}

				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

			

				for ( int i = 0; i < secret_digits_count; i++)
				{
					numsViewport[i].x = 40 + i*200;
					numsViewport[i].y = 300;
					numsViewport[i].w = 120;
					numsViewport[i].h = 200;
					SDL_RenderSetViewport( gRenderer, &numsViewport[i] );
					SDL_RenderCopy( gRenderer, gTextureNums[inputNums[i]], NULL, NULL );
				}

				for ( int i = 0; i < bulls; i++ )
				{
					cowsViewport[i].x = 10+i*90;
					cowsViewport[i].y = 10;
					cowsViewport[i].w = 80;
					cowsViewport[i].h = 80;
					SDL_RenderSetViewport( gRenderer, &cowsViewport[i] );
					SDL_RenderCopy( gRenderer, gTextureBull, NULL, NULL );
				}
				for ( int i = 0; i < cows; i++ )
				{
					bullsViewport[i].x = 440 + i*90;
					bullsViewport[i].y = 10;
					bullsViewport[i].w = 80;
					bullsViewport[i].h = 80;
					SDL_RenderSetViewport( gRenderer, &bullsViewport[i] );
					SDL_RenderCopy( gRenderer, gTextureCow, NULL, NULL );

				}

				SDL_RenderSetViewport( gRenderer, &numsViewport[select-1] );
				SDL_RenderCopy( gRenderer, currentNumTexture, NULL, NULL );

				SDL_RenderPresent( gRenderer );
			}

		}
	}

	close();

	return 0;
}