#ifndef _GAME_OVER_H_
#define _GAME_OVER_H_

#include <stdbool.h>
#include <stdint.h>

typedef enum {
  GAME_OVER_OPTION_NONE,
  GAME_OVER_OPTION_MAIN_MENU
} GameOverOption;

typedef enum {
  GAME_OVER_ACTION_NONE,
  GAME_OVER_ACTION_MAIN_MENU
} GameOverAction;

typedef struct {
  GameOverOption hovered_option;
  uint32_t final_score;
} game_over_t;

void game_over_init(game_over_t *game_over, uint32_t final_score);
GameOverAction game_over_update(game_over_t *game_over, GameOverOption hovered_option, bool clicked);

#endif /* _GAME_OVER_H_ */
