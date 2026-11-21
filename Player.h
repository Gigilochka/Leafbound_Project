#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "Constants.h"

struct Player {
    SDL_Texture* idleTexture = nullptr;
    SDL_Texture* skipTexture = nullptr;
    SDL_Texture* jumpTexture = nullptr;
    SDL_Texture* fallTexture = nullptr;
    SDL_Texture* climbTexture = nullptr;

    int skipFrameW = 0, skipFrameH = 0;
    int jumpFrameW = 0, jumpFrameH = 0;
    int fallFrameW = 0, fallFrameH = 0;
    int climbFrameW = 0, climbFrameH = 0;
    int climbCooldown = 0;

    SDL_Rect srcRect = { 0, 0, IDLE_FRAME_W, IDLE_FRAME_H };
    SDL_Rect destRect = { PLAYER_START_X, PLAYER_START_Y, PLAYER_WIDTH, PLAYER_HEIGHT };

    SDL_RendererFlip flip = SDL_FLIP_NONE;
    bool isMoving = false;
    bool isClimbing = false;

    float velocityY = 0.0f;
    bool isGrounded = true;
};

void InitPlayer(Player& player, SDL_Renderer* renderer);
void UpdatePlayer(Player& player, const Uint8* state, const SDL_Rect& ladderRect);
void RenderPlayer(const Player& player, SDL_Renderer* renderer, const SDL_Rect& camera);
void DestroyPlayer(Player& player);
