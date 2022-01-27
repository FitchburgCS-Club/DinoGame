#ifdef __unix__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#else
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_timer.h>
#endif
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "entities.h"
#include "errors.h"
#include "macros.h"

#define FPS 60

Dino dino;

bool running = 1;

Animation *getAnims(char *path) {
	FILE *animFile = fopen(path, "r");
	if (animFile == NULL) {
		perror("fopen");
		exit(ANIM_LOAD_ERR);
	}
	unsigned char nAnims;
	fread(&nAnims,1,1,animFile);
	Animation *anims = malloc(sizeof(Animation)*nAnims);
	for (unsigned char i = 0; i < nAnims; i++) {
		fread(&anims[i].nFrames, 1, 1, animFile);
		fread(&anims[i].fps, 1, 1, animFile);
		anims[i].frames = malloc(sizeof(SDL_Rect) * anims[i].nFrames);
		fread(anims[i].frames, sizeof(SDL_Rect), anims[i].nFrames, animFile);
	}
	return anims;
}

void lerpY(SDL_Rect *out, SDL_Rect *start, SDL_Rect *end, float f) {
	out->y = (float)start->y + ((float)end->y - (float)start->y) * f;
}

SDL_Rect jumpRect = {
	0,
	150,
	0,
	0
};

SDL_Rect groundRect = {
	0,
	208,
	0,
	0
};

const int jumpTimeLength = 1000; // NOTE(Michael): Total number of ticks jummping takes to complete

int main(int argc, char *argv[]) {
	// TODO: Add menu of some kind to change window size
	int scr_width = 640, scr_height = 480;
	SDL_Window* window = NULL;
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) < 0) {
		eprintf("Could not initialize SDL2: %s\n", SDL_GetError());
		return INIT_ERR;
	}
	int iFlags = IMG_INIT_PNG;
	if(IMG_Init(iFlags) != iFlags) {
		eprintf("Could not initialize SDL2_Image\n");
		return INIT_ERR;
	}
	window = SDL_CreateWindow(
			"Dino Game",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			scr_width, scr_height,
			SDL_WINDOW_SHOWN
			);
	if (window == NULL) {
		eprintf("Could not create window: %s\n", SDL_GetError());
		return GENERIC_ERR;
	}

	Uint32 render_flags = SDL_RENDERER_ACCELERATED;

	SDL_Renderer* rend = SDL_CreateRenderer(window, -1, render_flags);
	SDL_SetRenderDrawColor(rend, 153, 153, 255, 255);
	SDL_Surface *dinoSurface = IMG_Load("res/sprites/dinosheet.png");
	SDL_Texture* dinoTex = SDL_CreateTextureFromSurface(rend, dinoSurface);
	SDL_FreeSurface(dinoSurface);

	dino.pos.w = 64;
	dino.pos.h = 64;
	dino.anims = getAnims("res/animdata/dino.dat");
	dino.cAnim = &dino.anims[2];
	dino.jumping = 0;
	dino.ducking = 0;
	unsigned char dinoFrame = 0;

	// sets initial position of the dino
	dino.pos.x = (scr_width - dino.pos.w) / 2;
	dino.pos.y = (scr_height - dino.pos.h) / 2;

	// animation loop
	int starttime, endtime, deltatime;
	SDL_Event event;
	while (running) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
					running = 0;
					break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.scancode) {
				case SDL_SCANCODE_W:
				case SDL_SCANCODE_UP:
					dino.jumping = 1;
					dino.jumpStart = SDL_GetTicks();
					break;
				case SDL_SCANCODE_S:
				case SDL_SCANCODE_DOWN:
					if (!dino.ducking) {
						dino.ducking = 1;
					}
					break;
				default:
					break;
				}
				break;
			case SDL_KEYUP:
				switch (event.key.keysym.scancode) {
				case SDL_SCANCODE_DOWN:
					dino.ducking = 0;
					break;
				default:
					break;
				}
				break;
			}
		}
		
		if (dino.ducking && !dino.jumping) {
			dino.cAnim = &dino.anims[0];
		} else {
			dino.cAnim = &dino.anims[2];
		}

		if (dino.jumping) {
			int current = SDL_GetTicks();
			
			float factor = ((float)(current - dino.jumpStart))/jumpTimeLength;
			// NOTE(Michael): we want to move up, and then move back down.
			// So factor [0, 0.5) is mapped to the range [0, 1]
			// and factor (0.5, 1] is mapped to the range [1, 0]

			if (factor >= 1) { // End if we're past our range
				dino.pos.y = groundRect.y;
				dino.jumping = 0;
			}
			else {
				if (factor >= 0.5) {
					factor = 0.5 * 2 - (factor - 0.5) * 2; // maps (0.5, 1] to [1, 0]
				}
				else {
					factor *= 2; // maps [0, 0.5) to [0, 1]
				}
				lerpY(&dino.pos, &groundRect, &jumpRect, factor);
			}
		}
		
		starttime = SDL_GetTicks();
		dinoFrame = (starttime/(1000/dino.cAnim->fps)) % dino.cAnim->nFrames;

		SDL_RenderClear(rend);
		SDL_RenderCopy(rend, dinoTex, &dino.cAnim->frames[dinoFrame], &dino.pos);

		// triggers the double buffers
		// for multiple rendering
		SDL_RenderPresent(rend);

		endtime = SDL_GetTicks();
		deltatime = endtime - starttime;

		// Limit to FPS
		if (deltatime > (1000 / FPS)) {
		} else {
			SDL_Delay((1000 / FPS) - deltatime);
		}
	}

	// Cleanup
	SDL_DestroyTexture(dinoTex);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
