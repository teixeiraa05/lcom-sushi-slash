#ifndef _MENU_H_
#define _MENU_H_

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

typedef struct menu {
  MenuOption hovered_option;
  difficulty_t selected_difficulty;
} menu_t;

typedef enum {
  PAUSE_OPTION_NONE,
  PAUSE_OPTION_RESUME,
  PAUSE_OPTION_EXIT
} PauseOption;

typedef enum {
  PAUSE_ACTION_NONE,
  PAUSE_ACTION_RESUME,
  PAUSE_ACTION_EXIT
} PauseAction;

typedef struct {
  PauseOption hovered_option;
} pause_menu_t;

void menu_init(menu_t *menu);
MenuAction menu_update(menu_t *menu, MenuOption hovered_option, bool clicked);

void pause_menu_init(pause_menu_t *menu);
PauseAction pause_menu_update(pause_menu_t *menu, PauseOption hovered_option, bool clicked);

#endif /* _MENU_H_ */
