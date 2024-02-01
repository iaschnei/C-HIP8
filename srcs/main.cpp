#include "c_hip8.hpp"

int main (int ac, char **av) {

	if (ac != 2) {
		std::cerr << "Please input a ROM file." << std::endl;
		return (1);
	}

	(void) av;

	t_sdl_data		sdl_data;
	t_components	components;

	if (!initalise_SDL(&sdl_data)) {
		return (1);
	}

	if (!initialise_memory(&components, av[1])) {
		SDL_Quit();
		return (1);
	}

	while (true) {

		SDL_Event event;
		while (SDL_PollEvent(&event)) {

			if (event.type == SDL_QUIT) {
				SDL_Quit();
				return (0);
			}

			if (event.type == SDL_KEYDOWN) {
				for (int i = 0; i < 16; ++i) {
					if (event.key.keysym.sym == keymap[i]) {
						components.key_pressed[i] = 1;
					}
				}
			}

			if (event.type == SDL_KEYUP) {
				for (int i = 0; i < 16; ++i) {
					if (event.key.keysym.sym == keymap[i]) {
						components.key_pressed[i] = 0;
					}
				}
            }
		}

		if (!handle_operation(&components, &sdl_data)) {
			SDL_Quit();
			return (1);
		}

		update_display(&components, &sdl_data);

		std::this_thread::sleep_for(std::chrono::microseconds(1200));
	}

	SDL_Quit();
	return (0);
}