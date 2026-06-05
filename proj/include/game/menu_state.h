#ifndef _MENU_STATE_H_
#define _MENU_STATE_H_

#include <stdbool.h>

/**
 * @file menu_state.h
 * @brief Declares main menu state transitions.
 */

/**
 * @brief Selectable entries in the main menu.
 */
typedef enum {
  /** @brief No menu entry is selected. */
  MENU_OPTION_NONE,

  /** @brief Start singleplayer gameplay. */
  MENU_OPTION_SINGLEPLAYER,

  /** @brief Start multiplayer gameplay. */
  MENU_OPTION_MULTIPLAYER,

  /** @brief Show instructions. */
  MENU_OPTION_INSTRUCTIONS,

  /** @brief Exit the application. */
  MENU_OPTION_EXIT
} MenuOption;

/**
 * @brief Actions emitted by the main menu state machine.
 */
typedef enum {
  /** @brief No action should be taken. */
  MENU_ACTION_NONE,

  /** @brief Transition to singleplayer gameplay. */
  MENU_ACTION_START_SINGLEPLAYER,

  /** @brief Transition to multiplayer gameplay. */
  MENU_ACTION_START_MULTIPLAYER,

  /** @brief Transition to instructions. */
  MENU_ACTION_SHOW_INSTRUCTIONS,

  /** @brief Exit the application. */
  MENU_ACTION_EXIT
} MenuAction;

/**
 * @brief Mutable state for the main menu.
 */
typedef struct {
  /** @brief Option currently hovered by the input cursor. */
  MenuOption hovered_option;
} menu_t;

/**
 * @brief Initialize main menu state.
 *
 * @param menu Menu state to initialize.
 */
void menu_init(menu_t *menu);

/**
 * @brief Update main menu state from hover and click input.
 *
 * @param menu Menu state to mutate.
 * @param hovered_option Option currently under the cursor.
 * @param clicked Whether the user clicked this frame.
 * @return Action requested by the menu.
 */
MenuAction menu_update(menu_t *menu, MenuOption hovered_option, bool clicked);

#endif /* _MENU_STATE_H_ */
