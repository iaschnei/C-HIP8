#include "c_hip8.hpp"

static void	op_clear_screen(t_sdl_data *sdl_data);
static void op_jump_to(t_components *components, uint8_t nib1, uint8_t nib2, uint8_t nib3);
static void op_set_vx(t_components *components, uint8_t nib1, uint8_t nib2, uint8_t nib3);
static void op_add_to_vx(t_components *components, uint8_t nib1, uint8_t nib2, uint8_t nib3);
static void op_set_index(t_components *components, uint8_t nib1, uint8_t nib2, uint8_t nib3);
static void op_draw(t_components *components, t_sdl_data *sdl_data ,uint8_t nib1, uint8_t nib2, uint8_t nib3);


/*	
**	Probably the most important part of the program.
**	It will read the next operation in memory, pointed to by the program counter.
**	This instruction (2 bytes) will be divided in 4 nibbles. The nibbles determine which instruction to execute.
** 	Return 0 if the instruction is unknown, should never happen unless with homemade roms with errors
*/
int handle_operation(t_components *components, t_sdl_data *sdl_data) {

    uint8_t nibbles[4];
	uint8_t	instruction_byte;

	if (components->program_counter > 4096) {
		return (0);
	}

	std::cout << "Current operation: " << (int) components->memory[components->program_counter] << std::endl;

	instruction_byte = components->memory[components->program_counter];
	nibbles[0] = HI_NIBBLE(instruction_byte);
	nibbles[1] = LO_NIBBLE(instruction_byte);
	components->program_counter ++;
	instruction_byte = components->memory[components->program_counter];
	nibbles[2] = HI_NIBBLE(instruction_byte);
	nibbles[3] = LO_NIBBLE(instruction_byte);
	components->program_counter ++;

	switch(nibbles[0]) {

		case 0x0:
			if (nibbles[1] == 0x0 && nibbles[2] == 0xE && nibbles[3] == 0x0) {
				op_clear_screen(sdl_data);
			}
			else if (nibbles[1] == 0x0 && nibbles[2] == 0xE && nibbles[3] == 0xE) {
				//op_return_subroutine(components);
			}
			break;
		
		case 0x1:
			op_jump_to(components, nibbles[1], nibbles[2], nibbles[3]);
			break;

		case 0x6:
			op_set_vx(components, nibbles[1], nibbles[2], nibbles[3]);
			break;
		
		case 0x7:
			op_add_to_vx(components, nibbles[1], nibbles[2], nibbles[3]);
			break;

		case 0xA:
			op_set_index(components, nibbles[1], nibbles[2], nibbles[3]);
			break;

		case 0xD:
			op_draw(components, sdl_data, nibbles[1], nibbles[2], nibbles[3]);
			break;

		default:
			std::cerr << "Unknown opcode" << std::endl;
			return (0);
	}

    return (1);
}

// Reset the screen to "off" color
static void	op_clear_screen(t_sdl_data *sdl_data) {

	SDL_SetRenderDrawColor(sdl_data->renderer, OFF_COL_R, OFF_COL_G, OFF_COL_B, 255);

	SDL_SetRenderTarget(sdl_data->renderer, NULL);

	SDL_RenderClear(sdl_data->renderer);
}

// Set program counter to the specified address
static void op_jump_to(t_components *components, uint8_t nib1, uint8_t nib2, uint8_t nib3) {

	components->program_counter = (nib1 << 8) | (nib2 << 4) | nib3;
}

// Set register X to the specified value
static void op_set_vx(t_components *components, uint8_t nib1, uint8_t nib2, uint8_t nib3) {

	components->registers[nib1] = (nib2 << 4) | nib3;
}

// Add the specified value to register X
static void op_add_to_vx(t_components *components, uint8_t nib1, uint8_t nib2, uint8_t nib3) {

	components->registers[nib1] += (nib2 << 4) | nib3;
}

// Set the index register to the specified address
static void op_set_index(t_components *components, uint8_t nib1, uint8_t nib2, uint8_t nib3) {

	components->index_register = (nib1 << 8) | (nib2 << 4) | nib3;
}

// Draw a sprite to the screen of height nib3 and at coordinates nib1 nib2
// The sprite is located at index register in memory
static void op_draw(t_components *components, t_sdl_data *sdl_data ,uint8_t nib1, uint8_t nib2, uint8_t nib3) {

	uint8_t x_coord = components->registers[nib1] % RESOL_W;
	uint8_t y_coord = components->registers[nib2] % RESOL_H;

	uint16_t sprite_byte;

	components->registers[0xF] = 0;

	for (int line = 0; line < nib3; line++) {
		
		if (y_coord >= RESOL_H)
			break;

		sprite_byte = components->memory[components->index_register + line];

		x_coord = components->registers[nib1] % RESOL_W;

		for (int x_shift = 7; x_shift >= 0; x_shift--) {

			if (x_coord >= RESOL_W)
				break;

			if ((sprite_byte >> x_shift) & 1) {
				if (components->display[64 * y_coord + x_coord] == 1) {
					components->display[64 * y_coord + x_coord] = 0;
					components->registers[0xF] = 1;
				}
				else {
					components->display[64 * y_coord + x_coord] = 1;
				}
			}
			x_coord ++;
		}
		y_coord ++;
	}

	update_display(components, sdl_data);
}

// Update the display, switching colors on to off and off to on if necessary
void update_display(t_components *components, t_sdl_data *sdl_data) {

	SDL_SetRenderTarget(sdl_data->renderer, sdl_data->texture);

	for (int y = 0; y < 32; y++) {

		for (int x = 0; x < 64; x++) {
			
			if (components->display[64 * y + x] == 1) {
				SDL_SetRenderDrawColor(sdl_data->renderer, ON_COL_R, ON_COL_G, ON_COL_B, 255);
				SDL_RenderDrawPoint(sdl_data->renderer, x, y);
			}
			else {
				SDL_SetRenderDrawColor(sdl_data->renderer, OFF_COL_R, OFF_COL_G, OFF_COL_B, 255);
				SDL_RenderDrawPoint(sdl_data->renderer, x, y);
			}
		}
	}

	SDL_SetRenderTarget(sdl_data->renderer, NULL);
	SDL_RenderCopy(sdl_data->renderer, sdl_data->texture, NULL, NULL);
	SDL_RenderPresent(sdl_data->renderer);
}