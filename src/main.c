#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

//TODO (Zack): Make window size dynamically
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define FPS 60

int main(int argc, char *argv[]) {
	SDL_Window* window = NULL;
	SDL_Surface* screenSurface = SDL_CreateRGBSurface(0, 32, 32, 32, 0, 0, 0, 0);
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) < 0) {
		printf("could not initialize sdl2: %s\n", SDL_GetError());
		return 1;
	}
	window = SDL_CreateWindow(
			"Dino Game",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH, SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN
			);
	if (window == NULL) {
		printf("could not create window: %s\n", SDL_GetError());
		return 1;
	}

	Uint32 render_flags = SDL_RENDERER_ACCELERATED;

	SDL_Renderer* rend = SDL_CreateRenderer(window, -1, render_flags);
	SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0x00, 0x00));
	SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, screenSurface); // Square Texture
	SDL_FreeSurface(screenSurface);

	SDL_Rect dest;

	// connects our texture with dest to control position
	SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);

	// sets initial position of object
	dest.x = (SCREEN_WIDTH - dest.w) / 2;
	dest.y = (SCREEN_HEIGHT - dest.h) / 2;

	int running = 1;

	int speed = 300;

	// animation loop
	int starttime, endtime, deltatime;
	while (running) {
		SDL_Event event;
		starttime = SDL_GetTicks();
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					running = 0;
					break;

				case SDL_KEYDOWN:
					switch (event.key.keysym.scancode) {
						case SDL_SCANCODE_W:
						case SDL_SCANCODE_UP:
							dest.y -= speed / 30;
							break;
						case SDL_SCANCODE_A:
						case SDL_SCANCODE_LEFT:
							dest.x -= speed / 30;
							break;
						case SDL_SCANCODE_S:
						case SDL_SCANCODE_DOWN:
							dest.y += speed / 30;
							break;
						case SDL_SCANCODE_D:
						case SDL_SCANCODE_RIGHT:
							dest.x += speed / 30;
							break;
						default:
							break;
					}
					break;
			}
		}

		// right boundary
		if (dest.x + dest.w > SCREEN_WIDTH)
			dest.x = SCREEN_WIDTH - dest.w;

		// left boundary
		if (dest.x < 0)
			dest.x = 0;

		// bottom boundary
		if (dest.y + dest.h > SCREEN_HEIGHT)
			dest.y = SCREEN_HEIGHT - dest.h;

		// upper boundary
		if (dest.y < 0)
			dest.y = 0;

		SDL_RenderClear(rend);
		SDL_RenderCopy(rend, tex, NULL, &dest);

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
	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
