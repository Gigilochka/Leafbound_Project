#include "Player.h"
#include <iostream>

void InitPlayer(Player& player, SDL_Renderer* renderer) {
    player.idleTexture = IMG_LoadTexture(renderer, "assets/idle.png");
    if (player.idleTexture == nullptr) {
        std::cout << "Ошибка загрузки idle.png: " << IMG_GetError() << std::endl;
    }

    player.skipTexture = IMG_LoadTexture(renderer, "assets/skip.png");
    if (player.skipTexture == nullptr) {
        std::cout << "Ошибка загрузки skip.png: " << IMG_GetError() << std::endl;
    }

    int totalSkipW = 0, totalSkipH = 0;
    SDL_QueryTexture(player.skipTexture, NULL, NULL, &totalSkipW, &totalSkipH);
    player.skipFrameW = totalSkipW / SPRITE_SKIP_COUNT;
    player.skipFrameH = totalSkipH;

    player.jumpTexture = IMG_LoadTexture(renderer, "assets/jump.png");
    if (player.jumpTexture == nullptr) {
        std::cout << "Ошибка загрузки jump.png: " << IMG_GetError() << std::endl;
    }

    int totalJumpW = 0, totalJumpH = 0;
    SDL_QueryTexture(player.jumpTexture, NULL, NULL, &totalJumpW, &totalJumpH);
    player.jumpFrameW = totalJumpW / SPRITE_JUMP_COUNT;
    player.jumpFrameH = totalJumpH;

    player.fallTexture = IMG_LoadTexture(renderer, "assets/fall.png");
    if (player.fallTexture == nullptr) {
        std::cout << "Ошибка загрузки fall.png: " << IMG_GetError() << std::endl;
    }

    int totalFallW = 0, totalFallH = 0;
    SDL_QueryTexture(player.fallTexture, NULL, NULL, &totalFallW, &totalFallH);
    player.fallFrameW = totalFallW / SPRITE_FALL_COUNT;
    player.fallFrameH = totalFallH;

    player.destRect = { PLAYER_START_X, PLAYER_START_Y, PLAYER_WIDTH, PLAYER_HEIGHT };
    player.srcRect = { 0, 0, IDLE_FRAME_W, IDLE_FRAME_H };
    player.flip = SDL_FLIP_NONE;
    player.isMoving = false;
}

void UpdatePlayer(Player& player, const Uint8* state) {
    player.isMoving = false;
    SDL_RendererFlip oldFlip = player.flip;

    if ((state[SDL_SCANCODE_W] || state[SDL_SCANCODE_SPACE]) && player.isGrounded) {
        player.velocityY = JUMP_FORCE;
        player.isGrounded = false;
    }

    player.velocityY += GRAVITY;
    player.destRect.y += static_cast<int>(player.velocityY);

    if (player.destRect.y >= GROUND_Y) {
        player.destRect.y = GROUND_Y;
        player.velocityY = 0.0f;
        player.isGrounded = true;
    }

    if (state[SDL_SCANCODE_A]) {
        player.destRect.x -= PLAYER_SPEED;
        player.isMoving = true;
        player.flip = SDL_FLIP_HORIZONTAL;
    }

    if (state[SDL_SCANCODE_D]) {
        player.destRect.x += PLAYER_SPEED;
        player.isMoving = true;
        player.flip = SDL_FLIP_NONE;
    }

    if (state[SDL_SCANCODE_A] && state[SDL_SCANCODE_D]) {
        player.isMoving = false;
        player.flip = oldFlip;
    }

    if (player.destRect.x <= SCREEN_MARGIN) {
        player.destRect.x = SCREEN_MARGIN;
        player.isMoving = false;
    }
    if (player.destRect.x + player.destRect.w >= SCREEN_WIDTH - SCREEN_MARGIN) {
        player.destRect.x = SCREEN_WIDTH - SCREEN_MARGIN - player.destRect.w;
        player.isMoving = false;
    }
    if (player.destRect.y < 0) {
        player.destRect.y = 0;
    }
    if (player.destRect.y + player.destRect.h > SCREEN_HEIGHT) {
        player.destRect.y = SCREEN_HEIGHT - player.destRect.h;
    }

    if (!player.isGrounded) {
        if (player.velocityY < 0) {
            int frame = (SDL_GetTicks() / ANIMATION_JUMP_SPEED) % SPRITE_JUMP_COUNT;
            player.srcRect.x = frame * player.jumpFrameW;
            player.srcRect.y = 0;
            player.srcRect.w = player.jumpFrameW;
            player.srcRect.h = player.jumpFrameH;
        }
        else {
            int frame = (SDL_GetTicks() / ANIMATION_FALL_SPEED) % SPRITE_FALL_COUNT;
            player.srcRect.x = frame * player.fallFrameW;
            player.srcRect.y = 0;
            player.srcRect.w = player.fallFrameW;
            player.srcRect.h = player.fallFrameH;
        }
    }
    else if (player.isMoving) {
        int frame = (SDL_GetTicks() / ANIMATION_SKIP_SPEED) % SPRITE_SKIP_COUNT;
        player.srcRect.x = frame * player.skipFrameW;
        player.srcRect.y = 0;
        player.srcRect.w = player.skipFrameW;
        player.srcRect.h = player.skipFrameH;
    }
    else {
        int frame = (SDL_GetTicks() / ANIMATION_IDLE_SPEED) % SPRITE_IDLE_COUNT;
        player.srcRect.x = frame * IDLE_FRAME_W;
        player.srcRect.y = 0;
        player.srcRect.w = IDLE_FRAME_W;
        player.srcRect.h = IDLE_FRAME_H;
    }
}

void RenderPlayer(const Player& player, SDL_Renderer* renderer) {
    SDL_Texture* activeTexture = player.idleTexture;

    if (!player.isGrounded) {
        if (player.velocityY < 0) {
            activeTexture = player.jumpTexture;
        }
        else {
            activeTexture = player.fallTexture;
        }
    }
    else if (player.isMoving) {
        activeTexture = player.skipTexture;
    }

    if (activeTexture != nullptr) {
        SDL_RenderCopyEx(renderer, activeTexture, &player.srcRect, &player.destRect, 0.0, NULL, player.flip);
    }
}

void DestroyPlayer(Player& player) {
    if (player.idleTexture != nullptr) {
        SDL_DestroyTexture(player.idleTexture);
        player.idleTexture = nullptr;
    }
    if (player.skipTexture != nullptr) {
        SDL_DestroyTexture(player.skipTexture);
        player.skipTexture = nullptr;
    }
    if (player.jumpTexture != nullptr) {
        SDL_DestroyTexture(player.jumpTexture);
        player.jumpTexture = nullptr;
    }
    if (player.fallTexture != nullptr) {
        SDL_DestroyTexture(player.fallTexture);
        player.fallTexture = nullptr;
    }
}
