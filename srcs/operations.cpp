#include "c_hip8.hpp"

static void	op_clear_screen(t_components * components, t_sdl_data *sdl_data);
static void op_jump_to(t_components *components, uint8_t nib1, uint8_t nib2, uint8_t nib3);
static void op_set_vx(t_components *components, uint8_t nib1, uint8_t nib2, uint8_t nib3);
static void op_add_to_vx(t_components *components, uint8_t nib1, uint8_t nib2, uint8_t nib3);
static void op_set_index(t_components *components, uint8_t nib1, uint8_t nib2, uint8_t nib3);
static void op_draw(t_components *components, t_sdl_data *sdl_data ,uint8_t nib1, uint8_t nib2, uint8_t nib3);
static void op_return_from_subroutine(t_components *components);
static void op_call_subroutine(t_components *components, uint8_t nib1, uint8_t nib2, uint8_t nib3);
static void op_skip_if_X_equal(t_components *components, uint8_t nib1, uint8_t nib2, uint8_t nib3);
static void op_skip_if_X_unequal(t_components *components, uint8_t nib1, uint8_t nib2, uint8_t nib3);
static void op_skip_if_X_equal_Y(t_components *components, uint8_t nib1, uint8_t nib2);
static void op_skip_if_X_unequal_Y(t_components *components, uint8_t nib1, uint8_t nib2);
static void op_set_vx_to_vy(t_components * components, uint8_t nib1, uint8_t nib2);
static void op_set_vx_to_or_vy(t_components * components, uint8_t nib1, uint8_t nib2);
static void op_set_vx_to_and_vy(t_components * components, uint8_t nib1, uint8_t nib2);
static void op_set_vx_to_xor_vy(t_components * components, uint8_t nib1, uint8_t nib2);
static void op_add_vy_to_vx(t_components * components, uint8_t nib1, uint8_t nib2);
static void op_substract_vy_to_vx(t_components * components, uint8_t nib1, uint8_t nib2);
static void op_substract_vx_to_vy(t_components * components, uint8_t nib1, uint8_t nib2);
static void op_shift_vx_right(t_components * components, uint8_t nib1);
static void op_shift_vx_left(t_components * components, uint8_t nib1);
static void op_jump_v0(t_components *components, uint8_t nib1, uint8_t nib2, uint8_t nib3);
static void op_add_random(t_components *components, uint8_t nib1, uint8_t nib2, uint8_t nib3);
static void op_skip_if_pressed(t_components *components, uint8_t nib1);
static void op_skip_if_not_pressed(t_components *components, uint8_t nib1);
static void op_set_vx_to_timer(t_components *components, uint8_t nib1);
static void op_set_timer_to_vx(t_components *components, uint8_t nib1);
static void op_set_sound_to_vx(t_components *components, uint8_t nib1);
static void op_add_vx_to_index(t_components *components, uint8_t nib1);
static void op_wait_for_key(t_components *components, uint8_t nib1);
static void op_get_font(t_components *components, uint8_t nib1);
static void op_separate_decimals(t_components *components, uint8_t nib1);
static void op_store_memory(t_components *components, uint8_t nib1);
static void op_load_memory(t_components *components, uint8_t nib1);

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
				op_clear_screen(components, sdl_data);
			}
			else if (nibbles[1] == 0x0 && nibbles[2] == 0xE && nibbles[3] == 0xE) {
				op_return_from_subroutine(components);
			}
			break;
		
		case 0x1:
			op_jump_to(components, nibbles[1], nibbles[2], nibbles[3]);
			break;

		case 0x2:
			op_call_subroutine(components, nibbles[1], nibbles[2], nibbles[3]);
			break;

		case 0x3:
			op_skip_if_X_equal(components, nibbles[1], nibbles[2], nibbles[3]);
			break;

		case 0x4:
			op_skip_if_X_unequal(components, nibbles[1], nibbles[2], nibbles[3]);
			break;

		case 0x5:
			op_skip_if_X_equal_Y(components, nibbles[1], nibbles[2]);
			break ;

		case 0x6:
			op_set_vx(components, nibbles[1], nibbles[2], nibbles[3]);
			break;
		
		case 0x7:
			op_add_to_vx(components, nibbles[1], nibbles[2], nibbles[3]);
			break;

		case 0x8:
			switch(nibbles[3]) {

				case 0x0:
					op_set_vx_to_vy(components, nibbles[1], nibbles[2]);
					break;

				case 0x1:
					op_set_vx_to_or_vy(components, nibbles[1], nibbles[2]);
					break;

				case 0x2:
					op_set_vx_to_and_vy(components, nibbles[1], nibbles[2]);
					break;
				
				case 0x3:
					op_set_vx_to_xor_vy(components, nibbles[1], nibbles[2]);
					break;

				case 0x4:
					op_add_vy_to_vx(components, nibbles[1], nibbles[2]);
					break;
				
				case 0x5:
					op_substract_vy_to_vx(components, nibbles[1], nibbles[2]);
					break;

				case 0x6:
					op_shift_vx_right(components, nibbles[1]);
					break;

				case 0x7:
					op_substract_vx_to_vy(components, nibbles[1], nibbles[2]);
					break;
				
				case 0xE:
					op_shift_vx_left(components, nibbles[1]);
					break;
			}
			break;

		case 0x9:
			op_skip_if_X_unequal_Y(components, nibbles[1], nibbles[2]);
			break ;

		case 0xA:
			op_set_index(components, nibbles[1], nibbles[2], nibbles[3]);
			break;

		case 0xB:
			op_jump_v0(components, nibbles[1], nibbles[2], nibbles[3]);
			break;

		case 0xC:
			op_add_random(components, nibbles[1], nibbles[2], nibbles[3]);
			break;

		case 0xD:
			op_draw(components, sdl_data, nibbles[1], nibbles[2], nibbles[3]);
			break;

		case 0xE:
			switch (nibbles[3]) {
				
				case 0xE:
					op_skip_if_pressed(components, nibbles[1]);
					break;
				
				case 0x1:
					op_skip_if_not_pressed(components, nibbles[1]);
					break;
			}
			break;

		case 0xF:
			switch (nibbles[2]) {

				case 0x0:
					switch (nibbles[3]) {
					
						case 0x7:
							op_set_vx_to_timer(components, nibbles[1]);
							break;
						
						case 0xA:
							op_wait_for_key(components, nibbles[1]);
					}
					break;

				case 0x1:
					switch (nibbles[3]) {

						case 0x5:
							op_set_timer_to_vx(components, nibbles[1]);
							break;

						case 0x8:
							op_set_sound_to_vx(components, nibbles[1]);
							break;

						case 0xE:
							op_add_vx_to_index(components, nibbles[1]);
							break;
					}
					break;

				case 0x2:
					op_get_font(components, nibbles[1]);
					break;

				case 0x3:
					op_separate_decimals(components, nibbles[1]);
					break;

				case 0x5:
					op_store_memory(components, nibbles[1]);
					break;

				case 0x6:
					op_load_memory(components, nibbles[1]);
					break;
			}
			break;

		default:
			std::cerr << "Unknown opcode" << std::endl;
			return (0);
	}

	if (components->delay_timer > 0)
		components->delay_timer--;

	if (components->sound_timer > 0)
		components->sound_timer--;

    return (1);
}

// Reset the screen to "off" color
static void	op_clear_screen(t_components * components, t_sdl_data *sdl_data) {

	std::memset(components->display, 0, sizeof(components->display));

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

// Sets program_counter to the top element of stack
static void op_return_from_subroutine(t_components *components) {
	
	components->program_counter = components->function_stack.top();
	components->function_stack.pop();
}

// Similar to op_jump_to but first push current address to stack so we can return to it later
static void op_call_subroutine(t_components *components, uint8_t nib1, uint8_t nib2, uint8_t nib3) {

	components->function_stack.push(components->program_counter);
	components->program_counter = (nib1 << 8) | (nib2 << 4) | nib3;
}

// Skips an instruction is condition is met
static void op_skip_if_X_equal(t_components *components, uint8_t nib1, uint8_t nib2, uint8_t nib3) {
	
	if (components->registers[nib1] == ((nib2 << 4) | nib3)) {
		components->program_counter += 2;
	}
}

// Skips an instruction is condition is met
static void op_skip_if_X_unequal(t_components *components, uint8_t nib1, uint8_t nib2, uint8_t nib3) {
	
	if (components->registers[nib1] != ((nib2 << 4) | nib3)) {
		components->program_counter += 2;
	}
}

// Skips an instruction is condition is met
static void op_skip_if_X_equal_Y(t_components *components, uint8_t nib1, uint8_t nib2) {
	
	if (components->registers[nib1] == components->registers[nib2]) {
		components->program_counter += 2;
	}
}

// Skips an instruction is condition is met
static void op_skip_if_X_unequal_Y(t_components *components, uint8_t nib1, uint8_t nib2) {
	
	if (components->registers[nib1] != components->registers[nib2]) {
		components->program_counter += 2;
	}
}

// Sets register vx to register vy
static void op_set_vx_to_vy(t_components * components, uint8_t nib1, uint8_t nib2) {

	components->registers[nib1] = components->registers[nib2];
}

// Sets register vx to bitwise OR of vx and vy
static void op_set_vx_to_or_vy(t_components * components, uint8_t nib1, uint8_t nib2) {

	components->registers[nib1] = components->registers[nib1] | components->registers[nib2];
}

// Sets register vx to bitwise AND of vx and vy
static void op_set_vx_to_and_vy(t_components * components, uint8_t nib1, uint8_t nib2) {

	components->registers[nib1] = components->registers[nib1] & components->registers[nib2];
}

// Sets register vx to bitwise XOR of vx and vy
static void op_set_vx_to_xor_vy(t_components * components, uint8_t nib1, uint8_t nib2) {

	components->registers[nib1] = components->registers[nib1] ^ components->registers[nib2];
}

// Add vy to vx, check for overflow and set the flag accordingly
static void op_add_vy_to_vx(t_components * components, uint8_t nib1, uint8_t nib2) {

	int value1 = components->registers[nib1];
	int value2 = components->registers[nib2];

	components->registers[nib1] += components->registers[nib2];
	if (value2 > 255 - value1) {
		components->registers[0xF] = 1;
	}
	else {
		components->registers[0xF] = 0;
	}
}

// Substract vy to vx and store in vx
static void op_substract_vy_to_vx(t_components * components, uint8_t nib1, uint8_t nib2) {

	int value1 = components->registers[nib1];
	int value2 = components->registers[nib2];

	components->registers[nib1] = components->registers[nib1] - components->registers[nib2];
	if (value2 > value1) {
		components->registers[0xF] = 0;
	}
	else {
		components->registers[0xF] = 1;
	}
}

// Substract vx to vy and store in vx
static void op_substract_vx_to_vy(t_components * components, uint8_t nib1, uint8_t nib2) {

	int value1 = components->registers[nib1];
	int value2 = components->registers[nib2];

	components->registers[nib1] = components->registers[nib2] - components->registers[nib1];
	if (value1 > value2) {
		components->registers[0xF] = 0;
	}
	else {
		components->registers[0xF] = 1;
	}
}

// Shift vx 1 bit to the right
static void op_shift_vx_right(t_components * components, uint8_t nib1) {

	uint8_t	copy = components->registers[nib1];

	components->registers[nib1] = components->registers[nib1] >> 1;
	components->registers[0xF] = copy & 1;
	
}

// Shift vx 1 bit to the left
static void op_shift_vx_left(t_components * components, uint8_t nib1) {

	uint8_t	copy = components->registers[nib1];

	components->registers[nib1] = components->registers[nib1] << 1;
	components->registers[0xF] = (copy >> 7) & 1;
}

// Jump to specified location + value of v0
static void op_jump_v0(t_components *components, uint8_t nib1, uint8_t nib2, uint8_t nib3) {

	components->program_counter = ((nib1 << 8) | (nib2 << 4) | nib3) + components->registers[0x0];
}

// Add a random number (with AND nib2 | nib3) to vx
static void op_add_random(t_components *components, uint8_t nib1, uint8_t nib2, uint8_t nib3) {
	
	srand(time(NULL));
	components->registers[nib1] = (rand() % 16) & ((nib2 << 4) | nib3);
}

// Skip an instruction if specified key is pressed
static void op_skip_if_pressed(t_components *components, uint8_t nib1) {

	if (components->key_pressed[components->registers[nib1]]) {
		components->program_counter += 2;
	}
}

// Skip an instruction if specified key is NOT pressed
static void op_skip_if_not_pressed(t_components *components, uint8_t nib1) {

	if (!components->key_pressed[components->registers[nib1]]) {
		components->program_counter += 2;
	}
}

// Set register X to current timer value
static void op_set_vx_to_timer(t_components *components, uint8_t nib1) {

	components->registers[nib1] = components->delay_timer;
}

// Set timer to current register X value
static void op_set_timer_to_vx(t_components *components, uint8_t nib1) {

	components->delay_timer = components->registers[nib1];
}

// Set sound timer to current register X value (will play a sound when above 0)
static void op_set_sound_to_vx(t_components *components, uint8_t nib1) {

	components->sound_timer = components->registers[nib1];
}

// Add the value of vx to index register
static void op_add_vx_to_index(t_components *components, uint8_t nib1) {

	components->index_register += components->registers[nib1];

	if (components->index_register > 1000) {
		components->registers[0xF] = 1;
	}
}

// Loops until a key is pressed (decrement program counter to loop)
static void op_wait_for_key(t_components *components, uint8_t nib1) {

	for (int i = 0; i < 16; i ++) {

		if (components->key_pressed[i]) {
			components->registers[nib1] = i;
			return ;
		}
	}

	components->program_counter -= 2;
}

// Retrieve the specified font and set index register to its location
static void op_get_font(t_components *components, uint8_t nib1) {

	components->index_register = 0x050 + LO_NIBBLE(components->registers[nib1]) * 0x5;
}

// Separate a number into three (255 -> 2, 5 ,5)
static void op_separate_decimals(t_components *components, uint8_t nib1) {

	components->memory[components->index_register] = components->registers[nib1] / 100;

	components->memory[components->index_register + 1] = (components->registers[nib1] / 10) % 10;

	components->memory[components->index_register + 2] = components->registers[nib1] % 10;
}

// Store registers from 0 to X into memory
static void op_store_memory(t_components *components, uint8_t nib1) {

	for (int i = 0; i <= nib1; i ++) {
		components->memory[components->index_register + i] = components->registers[i];
	}
}

// Get memory and store in registers 0 to X
static void op_load_memory(t_components *components, uint8_t nib1) {

	for (int i = 0; i <= nib1; i ++) {
		components->registers[i] = components->memory[components->index_register + i];
	}
}