#ifndef C_HIP8_HPP
# define C_HIP8_HPP

# include <stack>
# include <stdint.h>
# include <iostream>
# include <unistd.h>
# include "SDL2/SDL.h"

const int SCREEN_W = 640;
const int SCREEN_H = 320;

const int OPERATION_PER_SEC = 700;
const int OPERATION_TICK = 1000 / OPERATION_PER_SEC;

// ------------------------------------------------------------------------------------------------

typedef struct s_components {


	uint8_t		registers[16];				// The registers of the processor

	uint16_t	index_register;				// Points to location in memory
	uint16_t    program_counter;			// Points to the current instruction in memory
	uint8_t		delay_timer;				// Will decrease from 60 to 0 every second
	uint8_t		sound_timer;				// Can go up to 60 aswell and decreases at the same rate, should play a sound when > 0

	std::stack<uint16_t> function_stack;	// Store return point from subroutines

	uint8_t		memory[4096];				// RAM available (4Kb)

	uint8_t		to_change[2048];			// Will be used as a buffer for changes on display (64x32)

}	t_components;

typedef struct s_sdl_data {

	SDL_Window		*window;
	SDL_Renderer	*renderer;
	SDL_Texture		*texture;

}	t_sdl_data;

// ------------------------------------------------------------------------------------------------

// ----------- Graphics :
int initalise_SDL(t_sdl_data *sdl_data);
int displayTest(t_sdl_data *sdl_data);

#endif