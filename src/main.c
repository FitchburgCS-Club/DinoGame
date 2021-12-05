#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#define FPS 60

int main(int argc, char *argv[]) {
	int scr_width = 640, scr_height = 480;
	SDL_Window* window = NULL;
	SDL_Surface* screenSurface = SDL_CreateRGBSurface(0, 32, 32, 32, 0, 0, 0, 0);
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) < 0) {
		printf("could not initialize sdl2: %s\n", SDL_GetError());
		return 1;
	}
	window = SDL_CreateWindow(
			"Dino Game",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			scr_width, scr_height,
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
	dest.x = (scr_width - dest.w) / 2;
	dest.y = (scr_height - dest.h) / 2;

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
						case SDL_SCANCODE_DELETE:
							SDL_GetWindowSize(window, &scr_width, &scr_height);
							switch (scr_width) {
								case 1280:
									SDL_SetWindowSize(window, 640, 480);
									break;
								case 640:
									SDL_SetWindowSize(window, 1280, 720);
									break;
								default:
									exit(1);
							}
							break;
						default:
							break;
					}
					break;
				case SDL_WINDOWEVENT:
					switch (event.window.event) {
						case SDL_WINDOWEVENT_SIZE_CHANGED:
							// Dummy printf because case statments have to start with expressions.
							printf("");
							int oW = scr_width, oH = scr_height;
							SDL_GetWindowSize(window, &scr_width, &scr_height);
							dest.x *= (scr_width / (float)oW);
							dest.y *= (scr_height / (float)oH);
							break;
					}
					break;
			}
		}

		// right boundary
		if (dest.x + dest.w > scr_width)
			dest.x = scr_width - dest.w;

		// left boundary
		if (dest.x < 0)
			dest.x = 0;

		// bottom boundary
		if (dest.y + dest.h > scr_height)
			dest.y = scr_height - dest.h;

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
