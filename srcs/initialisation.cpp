#include "c_hip8.hpp"

// Make sure all memory and pointers are empty / NULL
int initialise_memory(t_components *components, char *av1) {

	std::memset(components->registers, 0, sizeof(components->registers));

	std::memset(components->memory, 0, sizeof(components->memory));

	std::memset(components->display, 0, sizeof(components->display));

	components->index_register = 0;
	components->delay_timer = 60;
	components->sound_timer = 0;

	initialise_font(components);

	if (!load_rom(components, av1)) {
		return (0);
	}

	components->program_counter = 0x200;

	return (1);
}

// Store default font in memory from 0x050 to 0x09F (could be anywhere between 0 and 0x200)
void	initialise_font(t_components *components) {

	uint8_t	font[80] =
	{
		0xF0, 0x90, 0x90, 0x90, 0xF0, //0
		0x20, 0x60, 0x20, 0x20, 0x70, //1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
		0x90, 0x90, 0xF0, 0x10, 0x10, //4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
		0xF0, 0x10, 0x20, 0x40, 0x40, //7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
		0xF0, 0x90, 0xF0, 0x90, 0x90, //A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
		0xF0, 0x80, 0x80, 0x80, 0xF0, //C
		0xE0, 0x90, 0x90, 0x90, 0xE0, //D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
		0xF0, 0x80, 0xF0, 0x80, 0x80  //F
	};

	for (int i = 0x050; i <= 0x09F; i ++) {
		components->memory[i] = font[i - 0x050];
	}
}

// Load rom data into memory, starting at 0x200
int load_rom(t_components *components, char *av1) {

	std::ifstream fs;

	fs.open(av1, std::fstream::in);
	if (fs.fail()) {
		return (0);
	}

	fs.seekg(0, fs.end);
    int length = fs.tellg();
    fs.seekg(0, fs.beg);

	char *buffer = new char[length];

	fs.read(buffer, length);
	if (!fs) {
		delete[] buffer;
		return (0);
	}

	for (int i = 0; i < length && i + 0x200 < 4096; i++) {
		components->memory[i + 0x200] = (uint8_t)buffer[i];
	}

	fs.close();
	delete[] buffer;

	return (1);
}

// Create all components required by SDL to render on screen
int initalise_SDL(t_sdl_data *sdl_data) {

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cerr << "SDL_Init failure." << std::endl << SDL_GetError() << std::endl;
		return (0);
	}

	sdl_data->window = SDL_CreateWindow( "C-HIP 8",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SCREEN_W, SCREEN_H, 0
	);
	if (!sdl_data->window) {
		std::cerr << "SDL_CreateWindow failure." << std::endl << SDL_GetError() << std::endl;
		SDL_Quit();
		return (0);
	}

	sdl_data->renderer = SDL_CreateRenderer(sdl_data->window, -1, SDL_RENDERER_ACCELERATED);
	if (!sdl_data->renderer) {
		std::cerr << "SDL_CreateRenderer failure." << std::endl << SDL_GetError() << std::endl;
		SDL_Quit();
		return (0);
	}

	// Should not fail according to documentation, unless some params are wrong
	sdl_data->texture = SDL_CreateTexture(sdl_data->renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		64, 32
	);

	return (1);
}