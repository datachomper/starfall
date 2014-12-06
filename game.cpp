#include "SDL.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	SDL_Window *window;
	SDL_Surface *surface;
	SDL_Surface *image;

	if (SDL_Init(SDL_INIT_VIDEO)) {
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		return 1;
	}

	window = SDL_CreateWindow("boilerplate",
				  SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				  800, 600,
				  0);
	if (window == NULL) {
		printf("Oops, couldn't create window brah\n");
		return -1;
	}

	surface = SDL_GetWindowSurface(window);
	image = SDL_LoadBMP("unicorn.bmp");
	if (image == NULL) {
		printf("Unable to load unicorn: %s\n", SDL_GetError());
	}

	SDL_BlitScaled(image, NULL, surface, NULL);
	SDL_FreeSurface(image);

	SDL_UpdateWindowSurface(window);

	SDL_Delay(3000);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
