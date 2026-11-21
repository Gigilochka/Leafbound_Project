#include "Game.h"
#include "Constants.h"
#include <iostream>

bool InitGame(Game& game) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Ошибка SDL_Init: " << SDL_GetError() << std::endl;
        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cout << "Ошибка IMG_Init: " << IMG_GetError() << std::endl;
        return false;
    }

    game.window = SDL_CreateWindow(
        WINDOW_TITLE,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        0
    );
    if (game.window == nullptr) {
        std::cout << "Ошибка создания окна: " << SDL_GetError() << std::endl;
        return false;
    }

    game.renderer = SDL_CreateRenderer(game.window, -1, SDL_RENDERER_ACCELERATED);
    if (game.renderer == nullptr) {
        std::cout << "Ошибка создания рендера: " << SDL_GetError() << std::endl;
        return false;
    }

    game.bgTexture = IMG_LoadTexture(game.renderer, "assets/middleground.png");
    if (game.bgTexture == nullptr) {
        std::cout << "Ошибка загрузки фона: " << IMG_GetError() << std::endl;
    }

    InitPlayer(game.player, game.renderer);
    game.running = true;
    return true;
}

void RunGame(Game& game) {
    SDL_Event event;
    const Uint8* state = SDL_GetKeyboardState(NULL);

    while (game.running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                game.running = false;
            }
        }

        UpdatePlayer(game.player, state);

        SDL_SetRenderDrawColor(game.renderer, 240, 230, 140, 255);
        SDL_RenderClear(game.renderer);

        if (game.bgTexture != nullptr) {
            SDL_RenderCopy(game.renderer, game.bgTexture, nullptr, nullptr);
        }

        RenderPlayer(game.player, game.renderer);

        SDL_RenderPresent(game.renderer);
        SDL_Delay(16);
    }
}

void CleanupGame(Game& game) {
    DestroyPlayer(game.player);

    if (game.bgTexture != nullptr) {
        SDL_DestroyTexture(game.bgTexture);
        game.bgTexture = nullptr;
    }
    if (game.renderer != nullptr) {
        SDL_DestroyRenderer(game.renderer);
        game.renderer = nullptr;
    }
    if (game.window != nullptr) {
        SDL_DestroyWindow(game.window);
        game.window = nullptr;
    }

    IMG_Quit();
    SDL_Quit();
}
