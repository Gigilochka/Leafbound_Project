#include <SDL.h>
#include <iostream>
#include <SDL_image.h>

int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);

	SDL_Window* window = SDL_CreateWindow("Leafbound", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_Texture* bgTexture = IMG_LoadTexture(renderer, "assets/middleground.png");
	if (bgTexture == nullptr) {
		std::cout << "Ошибка загрузки фона: " << IMG_GetError() << std::endl;
	}

	bool running = true;
	SDL_Event event;

	while (running) 
{
		while (SDL_PollEvent(&event)) 
		{
			if (event.type == SDL_QUIT) {
				running = false;
			}
		}

		SDL_SetRenderDrawColor(renderer, 16, 185, 110, 255);
		SDL_RenderClear(renderer);

		if (bgTexture != nullptr) {
			SDL_RenderCopy(renderer, bgTexture, nullptr, nullptr);
		}

		SDL_RenderPresent(renderer);
		SDL_Delay(16);
	}
	
	SDL_DestroyTexture(bgTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();



	return 1;
}