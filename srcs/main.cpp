#include <stack>
#include <stdint.h>
#include <iostream>

#include <cstdlib>
#include <unistd.h>

#include "SDL2/SDL.h"

const int SCREEN_W = 640;
const int SCREEN_H = 320;

// ------------------------------------------------------------------------------------------------

typedef struct s_components {


	uint8_t		registers[16];				// The registers of the processor

	uint16_t	index_register;				// Points to location in memory
	uint16_t    program_counter;			// Points to the current instruction in memory
	uint8_t		delay_timer;				// Will decrease from 60 to 0 every second
	uint8_t		sound_timer;				// Can go up to 60 aswell and decreases at the same rate, should play a sound when > 0

	std::stack<uint16_t> function_stack;	// Store return point from subroutines

	uint8_t		memory[4096];				// RAM available (4Kb)

	uint8_t		screen[64][32];				// Display (can only be 1 or 0, on or off)

}	t_components;

// ------------------------------------------------------------------------------------------------

int main (int ac, char **av) {

	if (ac != 2) {
		std::cerr << "Please input a ROM file." << std::endl;
	}

	(void) av;

	// --- SDL initialisation ---

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cerr << "SDL_Init failure." << std::endl << SDL_GetError() << std::endl;
		return (1);
	}

	SDL_Window *window = SDL_CreateWindow( "C-HIP 8",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SCREEN_W, SCREEN_H, 0
	);

	if (!window) {
		std::cerr << "SDL_CreateWindow failure." << std::endl << SDL_GetError() << std::endl;
		SDL_Quit();
		return (2);
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

	if (!renderer) {
		std::cerr << "SDL_CreateRenderer failure." << std::endl << SDL_GetError() << std::endl;
		SDL_Quit();
		return (3);
	}

	// Should not fail according to documentation, unless some params are wrong
	SDL_Texture *texture = SDL_CreateTexture(renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		64, 32
	);
	
	// --------------------------

	uint32_t pixels[2048];
	int		row = 4;

	while (true) {

		SDL_Event event;
		while (SDL_PollEvent(&event)) {

			if (event.type == SDL_QUIT) {
				SDL_Quit();
				return (0);
			}
		}

		SDL_LockTexture(texture, NULL,(void **) &pixels, &row); //??

		//update pixels here
		pixels[rand() % 2048] = 0xff0000;

		SDL_UnlockTexture(texture);


		// Clear screen and render
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	SDL_Quit();
	return (0);
}