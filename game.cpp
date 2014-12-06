/* Copyright 2014 Russ Meyerriecks <datachomper@gmail.com> */
/* Quick game for Ludum Dare #31 */
/* Audio generated using BFXR http://www.bfxr.net/ */

#include "SDL.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FRAMERATE 60
#define MS_PER_FRAME ((1.0/FRAMERATE)*1000)
//#define MS_PER_FRAME 17
#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 300
#define MAX_ACTORS 1000

struct Actor {
	bool enabled;
	SDL_Rect rect;
};

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
				  WINDOW_WIDTH, WINDOW_HEIGHT,
				  0);
	if (window == NULL) {
		printf("Oops, couldn't create window brah\n");
		return -1;
	}

	surface = SDL_GetWindowSurface(window);

	/* Set background to black */
	SDL_FillRect(surface, &surface->clip_rect, 0);

	/* TODO temp make a star and move it */
	int velocity = 1; // Star falling speed in pixels/second
	Actor *actor = (Actor *)malloc(sizeof(actor[0]) * MAX_ACTORS);
	memset(actor, 0, sizeof(actor[0]) * MAX_ACTORS);

	bool running = true;
	while (running) {
		unsigned int start = SDL_GetTicks();

		/* Process events */
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				running = false;
			if (e.type == SDL_MOUSEBUTTONDOWN) {
				//printf("mouse x: %d y: %d\n", e.button.x, e.button.y);

				/* Detect clicking on an actor */
				for (int i = 0; i < MAX_ACTORS; i++) {
					if (actor[i].enabled) {
						if ((e.button.x >= actor[i].rect.x) &&
						    (e.button.x <= (actor[i].rect.x + actor[i].rect.w)) &&
						    (e.button.y >= actor[i].rect.y) &&
						    (e.button.y <= (actor[i].rect.y + actor[i].rect.h))) {

							actor[i].enabled = false;
						    }
					}
				}
			}
		}

		/* Update */
		/* Should we spawn actor this frame? */
		if ((rand() % 60) == 30) {
			/* Find the first free actor in the array */
			for (int i = 0; i < MAX_ACTORS; i++) {
				if (!actor[i].enabled) {
					/* Randomize spawn location */
					int x = (rand() % (WINDOW_WIDTH-10));
					actor[i].rect = {x, 0, 10, 10};
					actor[i].enabled = true;
					break;
				}
			}
		}

		/* Blank screen in prep for redraw */
		/* TODO Maybe dim the old background to make a smear effect */
		SDL_FillRect(surface, &surface->clip_rect, 0);

		for (int i = 0; i < MAX_ACTORS; i++) {
			if (actor[i].enabled) {
				if (actor[i].rect.y > WINDOW_HEIGHT) {
					actor[i].enabled = false;
				} else {
					actor[i].rect.y += velocity;
					SDL_FillRect(surface, &actor[i].rect, -1);
				}
			}
		}

		/* Render */
		SDL_UpdateWindowSurface(window);

		/* Sleep for any remaining time */
		int sleeptime = start + MS_PER_FRAME - SDL_GetTicks();
		if (sleeptime > 0) {
			//printf("Sleeping for %dms\n", sleeptime);
			SDL_Delay(sleeptime);
		}
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
