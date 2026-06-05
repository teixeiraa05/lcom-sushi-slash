#ifndef __GAME_H
#define __GAME_H

#include "boss.h"
#include "sushi.h"
#include "menu_state.h"
#include "pause_menu_state.h"
#include "game_over.h"
#include "sprite.h"
#include "waves.h"
#include "../../../lab4/mouse.h"

#define LIVES 3
#define MAX_ACTIVE_SUSHI 8
#define MAX_ACTIVE_FRAGMENTS 16

#define GAME_NORMAL_SCORE 15
#define GAME_HARD_SCORE 50

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
    sushi_t active_sushi[MAX_ACTIVE_SUSHI];
    sushi_fragment_t active_fragments[MAX_ACTIVE_FRAGMENTS];
    wave_t waves[WAVE_PATTERN_COUNT];
    wave_runtime_t wave_runtime;
    difficulty_t current_difficulty;
    uint8_t num_active_sushi;
    uint8_t num_active_fragments;
} gameplay_data_t;

typedef struct {
    GameState state;
    uint32_t score;
    uint8_t lives;
    sprite_t background;
    gameplay_data_t singleplayer;

    union {
        menu_t menu;

        pause_menu_t pause_menu;
        
        game_over_t game_over;

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
