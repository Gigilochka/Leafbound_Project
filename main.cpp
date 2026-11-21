#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include <vector>

int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);

	SDL_Window* window = SDL_CreateWindow("Leafbound", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_RendererFlip flip = SDL_FLIP_NONE;

	SDL_Texture* bgTexture = IMG_LoadTexture(renderer, "assets/middleground.png");
	if (bgTexture == nullptr) {
		std::cout << "Ошибка загрузки фона: " << IMG_GetError() << std::endl;
	}

	bool running = true;

	int idleFrames[] = { 0, 1, 2, 3 };
	const int SPRITE_IDLE_COUT = 4;
	const int ANIMATION_IDLE_SPEED = 200;
	
	SDL_Event event;

	SDL_Texture* idle = IMG_LoadTexture(renderer, "assets/idle.png");
	SDL_Rect idlerc = { 0, 0, 36, 29 };
	SDL_Rect person = { 20, 500, 100, 100 };
 
	const Uint8* state = SDL_GetKeyboardState(NULL);

	while (running) 
{
		while (SDL_PollEvent(&event)) 
		{
			switch (event.type)
			{
			case SDL_QUIT:
				running = false;
				break;
			default:
				break;
			}
		}

		bool isMoving = false;

		if (state[SDL_SCANCODE_W]) {
			person.y -= 5;
			isMoving = true;
		}
		if (state[SDL_SCANCODE_S]) {
			person.y += 5;
			isMoving = true;
		}
		if (state[SDL_SCANCODE_A]) {
			person.x -= 5;
			isMoving = true;
			flip = SDL_FLIP_HORIZONTAL;
		}
		if (state[SDL_SCANCODE_D]) {
			person.x += 5;
			isMoving = true;
			flip = SDL_FLIP_NONE;
		}

		const int Margin = 15;
		if (person.x < Margin) person.x = Margin;
		if (person.x + person.w > 800 - Margin) person.x = 800 - Margin - person.w;
		if (person.y < 0) person.y = 0;
		if (person.y + person.h > 600) person.y = 600 - person.h;
		
		if (!isMoving) {
			int index = (SDL_GetTicks() / ANIMATION_IDLE_SPEED) % SPRITE_IDLE_COUT;
			int frame = (SDL_GetTicks() / ANIMATION_IDLE_SPEED) % 4;
			idlerc.x = frame * 36;
		}
		else {
			idlerc.x = 0;
		}

		SDL_SetRenderDrawColor(renderer, 240, 230, 140, 255);
		SDL_RenderClear(renderer);

		if (bgTexture != nullptr) {
			SDL_RenderCopy(renderer, bgTexture, nullptr, nullptr);
		}

		SDL_RenderCopyEx(renderer, idle, &idlerc, &person, 0.0, NULL, flip);


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