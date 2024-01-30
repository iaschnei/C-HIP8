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

	int timer_count = 0;

	// Main program loop, will run OPERATION_PER_SEC times / sec
	// Check SDL_events then handle one operation at a time
	// Decrease timers if necessary then wait
	//  - Since handling an operation for such a simple system is almost instantaneous, usleep-ing is fine
	//  - It won't be extremely precise and execution speed could theoretically vary
	//	- Ultimately, it's a tradeoff between accuracy and code simplicity/readability

	while (true) {

		SDL_Event event;
		while (SDL_PollEvent(&event)) {

			if (event.type == SDL_QUIT) {
				SDL_Quit();
				return (0);
			}
		}

		if (!handle_operation(&components, &sdl_data)) {
			SDL_Quit();
			return (1);
		}

		timer_count ++;
		if (timer_count ==  DELAYRATE) {
			update_display(&components, &sdl_data);
			timer_count = 0;
			components.delay_timer --;
			if (components.delay_timer == 0) {
				components.delay_timer = 60;
			}
			
			if (components.sound_timer > 0) {
				components.sound_timer --;
			}
		}

		usleep(OPERATION_TICK * 1000);
	}

	SDL_Quit();
	return (0);
}