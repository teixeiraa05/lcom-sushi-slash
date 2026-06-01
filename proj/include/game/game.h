#ifndef __GAME_H
#define __GAME_H

#include "boss.h"
#include "sushi.h"
#include "difficulty.h"
#include "menu.h"
#include "sprite.h"

#define LIVES 3
#define MAX_ACTIVE_SUSHI 8
#define MAX_ACTIVE_FRAGMENTS 16
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
        } singleplayer;
        
        struct {
          sushi_t active_sushi[MAX_ACTIVE_SUSHI];
          sushi_fragment_t active_fragments[MAX_ACTIVE_FRAGMENTS];
          uint8_t num_active_sushi;
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


#endif /* __GAME_H */
