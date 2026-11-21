#include "Player.h"
#include <iostream>

static SDL_Texture* LoadTextureWithSize(SDL_Renderer* renderer, const char* path, int frames, int& frameW, int& frameH) {
    SDL_Texture* tex = IMG_LoadTexture(renderer, path);
    if (tex == nullptr) {
        std::cout << "Ошибка загрузки " << path << ": " << IMG_GetError() << std::endl;
        return nullptr;
    }
    int totalW = 0, totalH = 0;
    SDL_QueryTexture(tex, NULL, NULL, &totalW, &totalH);
    frameW = totalW / frames;
    frameH = totalH;
    return tex;
}

static void SetAnimationFrame(Player& player, int frameW, int frameH, int frameCount, int speed) {
    int frame = (SDL_GetTicks() / speed) % frameCount;
    player.srcRect = { frame * frameW, 0, frameW, frameH };
}

void InitPlayer(Player& player, SDL_Renderer* renderer) {
    int idleW = 0, idleH = 0;
    player.idleTexture = LoadTextureWithSize(renderer, "assets/idle.png", SPRITE_IDLE_COUNT, idleW, idleH);
    player.skipTexture = LoadTextureWithSize(renderer, "assets/skip.png", SPRITE_SKIP_COUNT, player.skipFrameW, player.skipFrameH);
    player.jumpTexture = LoadTextureWithSize(renderer, "assets/jump.png", SPRITE_JUMP_COUNT, player.jumpFrameW, player.jumpFrameH);
    player.fallTexture = LoadTextureWithSize(renderer, "assets/fall.png", SPRITE_FALL_COUNT, player.fallFrameW, player.fallFrameH);
    player.climbTexture = LoadTextureWithSize(renderer, "assets/climb.png", SPRITE_CLIMB_COUNT, player.climbFrameW, player.climbFrameH);

    player.destRect = { PLAYER_START_X, PLAYER_START_Y, PLAYER_WIDTH, PLAYER_HEIGHT };
    player.srcRect = { 0, 0, IDLE_FRAME_W, IDLE_FRAME_H };
}

static void UpdateClimbing(Player& player, const Uint8* state, const SDL_Rect& ladderRect) {
    if (player.climbCooldown > 0) player.climbCooldown--;

    SDL_Rect climbZone = { ladderRect.x + 5, ladderRect.y - 15, ladderRect.w / 3, ladderRect.h + 15 };
    bool onLadder = SDL_HasIntersection(&player.destRect, &climbZone);
    bool pressingSide = state[SDL_SCANCODE_A] || state[SDL_SCANCODE_D];

    if (!player.isClimbing && onLadder && !pressingSide && player.climbCooldown == 0) {
        bool wantUp = state[SDL_SCANCODE_W] && (player.destRect.y + player.destRect.h > ladderRect.y + 15);
        bool wantDown = state[SDL_SCANCODE_S] && (player.destRect.y < GROUND_Y);

        if (wantUp || wantDown) {
            player.isClimbing = true;
            player.isGrounded = false;
            player.velocityY = 0.0f;
            player.destRect.x = climbZone.x + climbZone.w / 2 - player.destRect.w / 2;
        }
    }

    if (!player.isClimbing) return;

    if (state[SDL_SCANCODE_SPACE]) {
        player.isClimbing = false;
        player.climbCooldown = 25;
        return;
    }

    if (state[SDL_SCANCODE_W]) player.destRect.y -= CLIMB_SPEED;
    if (state[SDL_SCANCODE_S]) player.destRect.y += CLIMB_SPEED;

    if (player.destRect.y + player.destRect.h <= ladderRect.y + 5) {
        player.destRect.y = ladderRect.y - player.destRect.h;
        player.isClimbing = false;
        player.isGrounded = true;
        player.climbCooldown = 25;
    }
    else if (player.destRect.y >= GROUND_Y) {
        player.destRect.y = GROUND_Y;
        player.isClimbing = false;
        player.isGrounded = true;
        player.climbCooldown = 15;
    }
    else if (!onLadder) {
        player.isClimbing = false;
    }
}

static void UpdateMovement(Player& player, const Uint8* state, const SDL_Rect& ladderRect) {
    int prevBottom = player.destRect.y + player.destRect.h;
    SDL_Rect climbZone = { ladderRect.x + 5, ladderRect.y - 15, ladderRect.w / 3, ladderRect.h + 15 };
    bool onLadder = SDL_HasIntersection(&player.destRect, &climbZone);

    if ((state[SDL_SCANCODE_W] || state[SDL_SCANCODE_SPACE]) && player.isGrounded && !onLadder) {
        player.velocityY = JUMP_FORCE;
        player.isGrounded = false;
    }

    player.velocityY += GRAVITY;
    player.destRect.y += static_cast<int>(player.velocityY);

    int currBottom = player.destRect.y + player.destRect.h;
    int centerX = player.destRect.x + player.destRect.w / 2;
    bool overPlatform = (centerX >= ladderRect.x && centerX <= ladderRect.x + ladderRect.w);

    if (player.velocityY >= 0 && overPlatform && prevBottom <= ladderRect.y + 12 && currBottom >= ladderRect.y) {
        player.destRect.y = ladderRect.y - player.destRect.h;
        player.velocityY = 0.0f;
        player.isGrounded = true;
    }
    else if (player.destRect.y >= GROUND_Y) {
        player.destRect.y = GROUND_Y;
        player.velocityY = 0.0f;
        player.isGrounded = true;
    }
    else {
        player.isGrounded = false;
    }

    SDL_RendererFlip oldFlip = player.flip;
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
}

static void UpdateAnimation(Player& player, const Uint8* state) {
    if (player.isClimbing) {
        int frame = (state[SDL_SCANCODE_W] || state[SDL_SCANCODE_S])
            ? (SDL_GetTicks() / ANIMATION_CLIMB_SPEED) % SPRITE_CLIMB_COUNT : 0;
        player.srcRect = { frame * player.climbFrameW, 0, player.climbFrameW, player.climbFrameH };
    }
    else if (!player.isGrounded) {
        if (player.velocityY < 0)
            SetAnimationFrame(player, player.jumpFrameW, player.jumpFrameH, SPRITE_JUMP_COUNT, ANIMATION_JUMP_SPEED);
        else
            SetAnimationFrame(player, player.fallFrameW, player.fallFrameH, SPRITE_FALL_COUNT, ANIMATION_FALL_SPEED);
    }
    else if (player.isMoving) {
        SetAnimationFrame(player, player.skipFrameW, player.skipFrameH, SPRITE_SKIP_COUNT, ANIMATION_SKIP_SPEED);
    }
    else {
        SetAnimationFrame(player, IDLE_FRAME_W, IDLE_FRAME_H, SPRITE_IDLE_COUNT, ANIMATION_IDLE_SPEED);
    }
}

void UpdatePlayer(Player& player, const Uint8* state, const SDL_Rect& ladderRect) {
    player.isMoving = false;

    UpdateClimbing(player, state, ladderRect);
    if (!player.isClimbing) {
        UpdateMovement(player, state, ladderRect);
    }

    if (player.destRect.x <= SCREEN_MARGIN) {
        player.destRect.x = SCREEN_MARGIN;
        player.isMoving = false;
    }
    if (player.destRect.x + player.destRect.w >= SCREEN_WIDTH - SCREEN_MARGIN) {
        player.destRect.x = SCREEN_WIDTH - SCREEN_MARGIN - player.destRect.w;
        player.isMoving = false;
    }
    if (player.destRect.y < 0) player.destRect.y = 0;

    UpdateAnimation(player, state);
}

void RenderPlayer(const Player& player, SDL_Renderer* renderer) {
    SDL_Texture* tex = player.idleTexture;
    if (player.isClimbing) tex = player.climbTexture;
    else if (!player.isGrounded) tex = (player.velocityY < 0) ? player.jumpTexture : player.fallTexture;
    else if (player.isMoving) tex = player.skipTexture;

    if (tex != nullptr) {
        SDL_RenderCopyEx(renderer, tex, &player.srcRect, &player.destRect, 0.0, NULL, player.flip);
    }
}

void DestroyPlayer(Player& player) {
    SDL_Texture* textures[] = { player.idleTexture, player.skipTexture, player.jumpTexture, player.fallTexture, player.climbTexture };
    for (SDL_Texture* tex : textures) {
        if (tex != nullptr) SDL_DestroyTexture(tex);
    }
}
