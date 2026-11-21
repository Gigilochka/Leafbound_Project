#include <SDL.h>
#include <SDL_image.h>
#include "Player.h"

struct Game {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* bgTexture = nullptr;
    Player player;
    bool running = false;
};

bool InitGame(Game& game);
void RunGame(Game& game);
void CleanupGame(Game& game);
