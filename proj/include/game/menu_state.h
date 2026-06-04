#ifndef _MENU_STATE_H_
#define _MENU_STATE_H_

#include <stdbool.h>
#include "difficulty.h"

typedef enum {
  MENU_OPTION_NONE,
  MENU_OPTION_SINGLEPLAYER,
  MENU_OPTION_MULTIPLAYER,
  MENU_OPTION_INSTRUCTIONS,
  MENU_OPTION_EXIT
} MenuOption;

typedef enum {
  MENU_ACTION_NONE,
  MENU_ACTION_START_SINGLEPLAYER,
  MENU_ACTION_START_MULTIPLAYER,
  MENU_ACTION_SHOW_INSTRUCTIONS,
  MENU_ACTION_EXIT
} MenuAction;

typedef struct {
  MenuOption hovered_option;
  difficulty_t selected_difficulty;
} menu_t;

void menu_init(menu_t *menu);
MenuAction menu_update(menu_t *menu, MenuOption hovered_option, bool clicked);

#endif /* _MENU_STATE_H_ */
