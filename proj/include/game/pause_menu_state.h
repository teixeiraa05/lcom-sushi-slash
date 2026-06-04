#ifndef _PAUSE_MENU_STATE_H_
#define _PAUSE_MENU_STATE_H_

#include <stdbool.h>

typedef enum {
  PAUSE_OPTION_NONE,
  PAUSE_OPTION_RESUME,
  PAUSE_OPTION_MAIN_MENU,
  PAUSE_OPTION_EXIT
} PauseOption;

typedef enum {
  PAUSE_ACTION_NONE,
  PAUSE_ACTION_RESUME,
  PAUSE_ACTION_MAIN_MENU,
  PAUSE_ACTION_EXIT
} PauseAction;

typedef struct {
  PauseOption hovered_option;
} pause_menu_t;

void pause_menu_init(pause_menu_t *menu);
PauseAction pause_menu_update(pause_menu_t *menu, PauseOption hovered_option, bool clicked);

#endif /* _PAUSE_MENU_STATE_H_ */
