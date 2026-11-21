#include <SDL.h>
#include <SDL_image.h>
#include "Player.h"

struct Game {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* bgTexture = nullptr;
    SDL_Texture* ladderTexture = nullptr;
    SDL_Rect ladderRect = { 350, 420, 160, 180 };
    
    Player player;
    bool running = false;
};

bool InitGame(Game& game);
void RunGame(Game& game);
void CleanupGame(Game& game);
