#include <SDL.h>

int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	bool running = true;
	SDL_Event event;
	while (running) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = false;
			}
		}
		SDL_Delay(16);
	}
	SDL_DestroyWindow(window);





	return 1;
}