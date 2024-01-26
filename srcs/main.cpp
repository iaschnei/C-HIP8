#include "c_hip8.hpp"

int main (int ac, char **av) {

	if (ac != 2) {
		std::cerr << "Please input a ROM file." << std::endl;
	}

	(void) av;

	t_sdl_data		sdl_data;
	t_components	components;

	if (!initalise_SDL(&sdl_data)) {
		return (1);
	}

	while (true) {

		SDL_Event event;
		while (SDL_PollEvent(&event)) {

			if (event.type == SDL_QUIT) {
				SDL_Quit();
				return (0);
			}
		}

		handle_operation(&components, &sdl_data);

		usleep(OPERATION_TICK * 1000);
	}

	SDL_Quit();
	return (0);
}