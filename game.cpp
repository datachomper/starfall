/* Copyright 2014 Russ Meyerriecks <datachomper@gmail.com> */
/* Quick game for Ludum Dare #31 */
/* Audio generated using BFXR http://www.bfxr.net/ */
/* Soundtrack from http://en.wikipedia.org/wiki/File:Holst-_mars.ogg */
/* Background based off http://commons.wikimedia.org/wiki/File:Nasa_blue_marble.jpg */

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FRAMERATE 60
#define MS_PER_FRAME ((1.0/FRAMERATE)*1000)
#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 300
#define MAX_ACTORS 1000

struct Actor {
	bool enabled;
	SDL_Rect rect;
};

enum Gamestate { TITLE_INTRO,TITLE, PLAY_TRANSITION, PLAYING, GAME_OVER };

int main(int argc, char **argv)
{
	SDL_Window *window;
	SDL_Surface *surface;
	SDL_Surface *image;
	SDL_Renderer *renderer;

	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)) {
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		return 1;
	}

	TTF_Init();
	TTF_Font *font = TTF_OpenFont("impact.ttf", 12);
	if (!font) {
		printf("font: %s\n", TTF_GetError());
	}

	//Mix_Init();
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	Mix_Chunk *explosion = Mix_LoadWAV("explosion.wav");
	Mix_Music *music = Mix_LoadMUS("mars.ogg");
	Mix_PlayMusic(music, -1);

	window = SDL_CreateWindow("starfall",
				  SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				  WINDOW_WIDTH, WINDOW_HEIGHT,
				  0);
	if (window == NULL) {
		printf("Oops, couldn't create window brah\n");
		return -1;
	}

	surface = SDL_GetWindowSurface(window);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);

	srand(time(NULL));

	/* Load background textures */
	SDL_Surface *title = SDL_LoadBMP("title_screen.bmp");
	SDL_Texture *title_texture = SDL_CreateTextureFromSurface(renderer, title);
	SDL_FreeSurface(title);

	SDL_FillRect(surface, &surface->clip_rect, 0);
	SDL_Surface *earth = SDL_LoadBMP("earth.bmp");
	SDL_Texture *earth_texture = SDL_CreateTextureFromSurface(renderer, earth);
	SDL_FreeSurface(earth);
	SDL_Rect earth_position = {0, 400, 300, 600};

	int velocity = 1; // Star falling speed in pixels/second
	Actor *actor = (Actor *)malloc(sizeof(actor[0]) * MAX_ACTORS);


	/* Setup score display */
	unsigned int score = 0;
	char score_text[256];
	SDL_Color grey = {200, 200, 200};
	SDL_Rect score_rect = {0, 0, 100, 20};

	Gamestate gamestate = TITLE_INTRO;

	int hits = 0;
	SDL_Rect earth_pos = {0, WINDOW_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT};

	bool running = true;
	while (running) {
		unsigned int start = SDL_GetTicks();

		/* Process events */
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				running = false;
			if (e.type == SDL_MOUSEBUTTONDOWN) {
				if (gamestate == TITLE) {
					/* Click enters into play game state */
					gamestate = PLAY_TRANSITION;
					break;
				} else if (gamestate == PLAYING) {
					/* Detect clicking on an actor */
					for (int i = 0; i < MAX_ACTORS; i++) {
						if (actor[i].enabled) {
							if ((e.button.x >= actor[i].rect.x) &&
							    (e.button.x <= (actor[i].rect.x + actor[i].rect.w)) &&
							    (e.button.y >= actor[i].rect.y) &&
							    (e.button.y <= (actor[i].rect.y + actor[i].rect.h))) {

								actor[i].enabled = false;
								score++;
								Mix_PlayChannel(-1, explosion, 0);
							}
						}
					}

				} else if (gamestate == GAME_OVER) {
					gamestate = TITLE;
				}
			}
		}

		/* Update */
		if (gamestate == TITLE_INTRO) {
			/* Scroll earth from bottom to top */
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
			SDL_RenderClear(renderer);
			if ((earth_pos.y -= 4) >= 0) {
				SDL_RenderCopy(renderer, title_texture, NULL, &earth_pos);
			} else {
				gamestate = TITLE;
			}
		} else if (gamestate == TITLE) {
			/* Draw title screen, then wait for clicks */
			SDL_RenderCopy(renderer, title_texture, NULL, NULL);

			/* Reset game state to start */
			hits = 0;
			score = 0;
			memset(actor, 0, sizeof(actor[0]) * MAX_ACTORS);
		} else if (gamestate == PLAY_TRANSITION) {
			/* Put neat title to playing animation effect here */
			gamestate = PLAYING;
		} else if (gamestate == PLAYING) {
			/* Should we spawn actor this frame? */
			if ((rand() % 60) == 30) {
				/* Find the first free actor in the array */
				for (int i = 0; i < MAX_ACTORS; i++) {
					if (!actor[i].enabled) {
						/* Randomize spawn location */
						int x = (rand() % (WINDOW_WIDTH-20));
						actor[i].rect = {x, 0, 20, 20};
						actor[i].enabled = true;
						break;
					}
				}
			}

			/* Blank screen in prep for redraw */
			/* TODO Maybe dim the old background to make a smear effect */
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, earth_texture, NULL, &earth_position);

			SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
			for (int i = 0; i < MAX_ACTORS; i++) {
				if (actor[i].enabled) {
					/* Detect actor hit the earth or apply velocity */
					if (actor[i].rect.y > WINDOW_HEIGHT) {
						actor[i].enabled = false;
						if (++hits >= 3) {
							/* Game over condition */
							gamestate = GAME_OVER;
						}
					} else {
						actor[i].rect.y += velocity;
						SDL_RenderFillRect(renderer, &actor[i].rect);
					}
				}
			}

			/* Adjust score and render */
			sprintf(&score_text[0], "score: %d", score);
			SDL_Surface *score = TTF_RenderText_Solid(font, score_text, grey);
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, score);
			SDL_RenderCopy(renderer, texture, NULL, &score_rect);
			SDL_FreeSurface(score);
			SDL_DestroyTexture(texture);

		} else if (gamestate == GAME_OVER) {
			/* Freeze game & display game over message */
			char text[256] = "Game Over";
			SDL_Rect rect = {0, WINDOW_HEIGHT/2, WINDOW_WIDTH, 20};
			SDL_Surface *a = TTF_RenderText_Solid(font, text, grey);
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, a);
			SDL_RenderCopy(renderer, texture, NULL, &rect);
			SDL_FreeSurface(a);
			SDL_DestroyTexture(texture);
		}

		/* Render */
		SDL_RenderPresent(renderer);

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
