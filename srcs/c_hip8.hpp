#ifndef C_HIP8_HPP
# define C_HIP8_HPP

# include <stack>
# include <stdint.h>
# include <iostream>
# include <unistd.h>
# include <cstring>
# include <istream>
# include <fstream>
# include <thread>
# include <chrono>
# include "SDL2/SDL.h"

# define HI_NIBBLE(b) (((b) >> 4) & 0x0F)
# define LO_NIBBLE(b) ((b) & 0x0F)

# define OFF_COL_R 31
# define OFF_COL_G 32
# define OFF_COL_B 34

# define ON_COL_R 251
# define ON_COL_G 250
# define ON_COL_B 245

const int SCREEN_W = 640;
const int SCREEN_H = 320;

const int RESOL_W = 64;
const int RESOL_H = 32;

const int OPERATION_PER_SEC = 660;
const int OPERATION_TICK = 1000 / OPERATION_PER_SEC;
const int DELAYRATE = 60;

const uint8_t keymap[16] = {		// Corresponds to the 16 keys to the left of a QWERTY keyboard, but in the original COSMAC VIP order
    SDLK_x,
    SDLK_1,
    SDLK_2,
    SDLK_3,
    SDLK_q,
    SDLK_w,
    SDLK_e,
    SDLK_a,
    SDLK_s,
    SDLK_d,
    SDLK_z,
    SDLK_c,
    SDLK_4,
    SDLK_r,
    SDLK_f,
    SDLK_v,
};

// ------------------------------------------------------------------------------------------------
// Since there is a limited amount of variables to keep track off, creating classes felt like an overkill on this one
// So I kept it to C-style structs !

typedef struct s_components {

	uint8_t		key_pressed[16];			// Indicates whether one of the 16 keys is pressed or not

	uint8_t		registers[16];				// The registers of the processor

	uint16_t	index_register;				// Points to location in memory
	uint16_t    program_counter;			// Points to the current instruction in memory
	uint8_t		delay_timer;				// Will decrease from 60 to 0 every second
	uint8_t		sound_timer;				// Can go up to 60 aswell and decreases at the same rate, should play a sound when > 0

	std::stack<uint16_t> function_stack;	// Store return point from subroutines

	uint8_t		memory[4096];				// RAM available (4Kb)

	uint8_t		display[2048];				// Will be used as a buffer for changes on display (64x32)

}	t_components;

typedef struct s_sdl_data {

	SDL_Window		*window;
	SDL_Renderer	*renderer;
	SDL_Texture		*texture;

}	t_sdl_data;

// ------------------------------------------------------------------------------------------------

// ----------- Initialisation (except SDL) :
int 	initialise_memory(t_components *components, char *av1);
void	initialise_font(t_components *components);
int 	initalise_SDL(t_sdl_data *sdl_data);
int 	load_rom(t_components *components, char *av1);


// ----------- Operations :
int 	handle_operation(t_components *components, t_sdl_data *sdl_data);
void 	update_display(t_components *components, t_sdl_data *sdl_data);

#endif