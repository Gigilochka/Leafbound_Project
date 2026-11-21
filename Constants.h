#pragma once

// Настройки окна
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const char* const WINDOW_TITLE = "Leafbound";

// Параметры игрока
const int PLAYER_SPEED = 5;
const int SCREEN_MARGIN = 15;
const int PLAYER_START_X = 20;
const int PLAYER_START_Y = 500;
const int PLAYER_WIDTH = 100;
const int PLAYER_HEIGHT = 100;

// Настройки анимации покоя (idle)
const int SPRITE_IDLE_COUNT = 4;
const int ANIMATION_IDLE_SPEED = 200;
const int IDLE_FRAME_W = 36;
const int IDLE_FRAME_H = 29;

// Настройки анимации передвижения (skip)
const int SPRITE_SKIP_COUNT = 8;
const int ANIMATION_SKIP_SPEED = 100; // чуть быстрее, чем idle, для плавного бега
const int SKIP_FRAME_W = 36;          // совпадает с твоей сеткой в GIMP
const int SKIP_FRAME_H = 29;

// Физика прыжка
const float GRAVITY = 0.6f;
const float JUMP_FORCE = -13.0f;
const int GROUND_Y = 500;

// Настройки анимации прыжка
const int SPRITE_JUMP_COUNT = 4;
const int ANIMATION_JUMP_SPEED = 120;

// Настройка анимации падения
const int SPRITE_FALL_COUNT = 4;
const int ANIMATION_FALL_SPEED = 120;

// Настройка анимаций подъема по лестнице
const int SPRITE_CLIMB_COUNT = 4;
const int ANIMATION_CLIMB_SPEED = 140;
const int CLIMB_SPEED = 3;

// Настройка экрана
const int LEVEL_WIDTH = 4000;




