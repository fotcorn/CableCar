#include <SDL2/SDL.h>

#include <iostream>

int main(int argc, char* args[]) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "Failed to initialize video: " << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow(
		"CableCar",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		1024, 786,
		SDL_WINDOW_SHOWN
	);
	if (window == nullptr) {
		std::cerr << "Failed to create window: " << SDL_GetError();
		return 1;
	}

	SDL_Surface* surface = SDL_GetWindowSurface(window);

	SDL_Event event; 
	bool running = true;
	while (running) {
		SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF));
		SDL_UpdateWindowSurface(window);

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				running = false;
				break;
			}
		}
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
