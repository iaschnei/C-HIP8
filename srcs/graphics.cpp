#include "c_hip8.hpp"

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

int displayTest(t_sdl_data *sdl_data) {

	SDL_SetRenderDrawColor(sdl_data->renderer, 255, 255, 255, 255); 			//Set color to white

	SDL_SetRenderTarget(sdl_data->renderer, sdl_data->texture);				//Render target is texture now

	SDL_RenderDrawPoint(sdl_data->renderer, 10, 10);							//Draw a point on render target -> texture here

	SDL_SetRenderTarget(sdl_data->renderer, NULL);							//Render tarfet is window now (NULL)

	SDL_RenderCopy(sdl_data->renderer, sdl_data->texture, NULL, NULL);		//Copy whole texture to render target -> window here
	SDL_RenderPresent(sdl_data->renderer);									//Update render

	return (1);
}