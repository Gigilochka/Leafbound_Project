#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "Constants.h"

struct Player {
    SDL_Texture* idleTexture = nullptr;
    SDL_Texture* skipTexture = nullptr;
    SDL_Texture* jumpTexture = nullptr;
    SDL_Texture* fallTexture = nullptr;

    int skipFrameW = 0;
    int skipFrameH = 0;
    int jumpFrameW = 0;
    int jumpFrameH = 0;
    int fallFrameW = 0;
    int fallFrameH = 0;

    SDL_Rect srcRect = { 0, 0, IDLE_FRAME_W, IDLE_FRAME_H };
    SDL_Rect destRect = { PLAYER_START_X, PLAYER_START_Y, PLAYER_WIDTH, PLAYER_HEIGHT };

    SDL_RendererFlip flip = SDL_FLIP_NONE;
    bool isMoving = false;

    float velocityY = 0.0f;
    bool isGrounded = true;

};

void InitPlayer(Player& player, SDL_Renderer* renderer);
void UpdatePlayer(Player& player, const Uint8* state);
void RenderPlayer(const Player& player, SDL_Renderer* renderer);
void DestroyPlayer(Player& player);
