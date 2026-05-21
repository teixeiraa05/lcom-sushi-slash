#ifndef __GAME_H
#define __GAME_H

#include "boss.h"
#include "entities.h"
#include "difficulty.h"

#define LIVES 3

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


int game_loop();


#endif /* __GAME_H */
