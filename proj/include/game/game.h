#ifndef __GAME_H
#define __GAME_H

#include "boss.h"
#include "sushi.h"
#include "difficulty.h"

#define LIVES 3
#define MAX_ACTIVE_SUSHI 8
#define MAX_ACTIVE_FRAGMENTS 16
typedef enum {
  MENU,
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
    boss_t active_boss;
    difficulty_t difficulty;

    //-- Sushi Pools--
    sushi_t sushi_pool[MAX_ACTIVE_SUSHI]; // Pool of sushi for singleplayer mode
    sushi_t fragment_pool[MAX_ACTIVE_FRAGMENTS]; // Pool of sushi for multiplayer mode
} GameInfo;

int game_loop();


#endif /* __GAME_H */
