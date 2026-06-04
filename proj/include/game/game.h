#ifndef __GAME_H
#define __GAME_H

#include "boss.h"
#include "sushi.h"
#include "difficulty.h"
#include "menu.h"
#include "sprite.h"
#include "../../../lab4/mouse.h"

#define LIVES 3
#define MAX_ACTIVE_SUSHI 8
#define MAX_ACTIVE_FRAGMENTS 16
#define TEST_SUSHI_SPAWN_TICKS 80

// Speed and gravity settings for different score thresholds
#define GAME_SPEED_1 22
#define GAME_SPEED_2 24
#define GAME_SPEED_3 26
#define GAME_SPEED_4 28
#define GAME_SPEED_5 30

#define GAME_GRAVITY_1 PHYSICS_FROM_THOUSANDTHS(500)
#define GAME_GRAVITY_2 PHYSICS_FROM_THOUSANDTHS(550)
#define GAME_GRAVITY_3 PHYSICS_FROM_THOUSANDTHS(620)
#define GAME_GRAVITY_4 PHYSICS_FROM_THOUSANDTHS(700)
#define GAME_GRAVITY_5 PHYSICS_FROM_THOUSANDTHS(790)

#define GAME_SPEED_2_SCORE 15
#define GAME_SPEED_3_SCORE 30
#define GAME_SPEED_4_SCORE 50
#define GAME_SPEED_5_SCORE 80

typedef enum {
  MENU,
  GAME,
  SINGLEPLAYER,
  MULTIPLAYER,
  PAUSED,
  WAITING,
  BOSS,
  GAME_OVER,
  EXIT
} GameState;

typedef enum {
  KEYBOARD,
  MOUSE,
  TIMER,
  RTC, 
  GRAPHICS,
  SERIALPORT
} Device;

typedef struct {
    GameState state;
    uint32_t score;
    uint8_t lives;
    sprite_t background;
    difficulty_t difficulty;

    union {
        menu_t menu;

        pause_menu_t pause_menu;
        
        struct {
          sprite_t game_over_background;
          uint32_t final_score;
        } game_over_sp; 

        struct {
          sushi_t active_sushi[MAX_ACTIVE_SUSHI];
          sushi_fragment_t active_fragments[MAX_ACTIVE_FRAGMENTS];
          uint8_t num_active_sushi;
          uint8_t num_active_fragments;
        } singleplayer;
        
        struct {
          sushi_t active_sushi[MAX_ACTIVE_SUSHI];
          sushi_fragment_t active_fragments[MAX_ACTIVE_FRAGMENTS];
          uint8_t num_active_sushi;
          uint8_t num_active_fragments;
          uint32_t player1_score;
          uint32_t player2_score;
        } multiplayer;

        struct {
          boss_t boss;
          sushi_fragment_t active_fragments[MAX_ACTIVE_FRAGMENTS];
          uint8_t num_active_fragments;
        } boss_fight;
    } data;
} GameInfo;

int game_loop();

void gameplay_clear(GameInfo *game);
void gameplay_start(GameInfo *game);
void gameplay_update(GameInfo *game, int current_tick, int *last_spawn_tick);
void gameplay_draw(const GameInfo *game);
void gameplay_handle_cut(GameInfo *game, MouseGameState *mouse_state);

#endif /* __GAME_H */
