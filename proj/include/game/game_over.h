#ifndef _GAME_OVER_H_
#define _GAME_OVER_H_

#include <stdbool.h>
#include <stdint.h>

/**
 * @file game_over.h
 * @brief Declares game-over screen state transitions.
 */

/**
 * @brief Selectable entries in the game-over screen.
 */
typedef enum {
  /** @brief No game-over entry is selected. */
  GAME_OVER_OPTION_NONE,

  /** @brief Return to the main menu. */
  GAME_OVER_OPTION_MAIN_MENU
} GameOverOption;

/**
 * @brief Actions emitted by the game-over state machine.
 */
typedef enum {
  /** @brief No action should be taken. */
  GAME_OVER_ACTION_NONE,

  /** @brief Return to the main menu. */
  GAME_OVER_ACTION_MAIN_MENU
} GameOverAction;

/**
 * @brief Mutable state for the game-over screen.
 */
typedef struct {
  /** @brief Option currently hovered by the input cursor. */
  GameOverOption hovered_option;

  /** @brief Score shown on the game-over screen. */
  uint32_t final_score;
} game_over_t;

/**
 * @brief Initialize game-over state.
 *
 * @param game_over Game-over state to initialize.
 * @param final_score Score to display.
 */
void game_over_init(game_over_t *game_over, uint32_t final_score);

/**
 * @brief Update game-over state from hover and click input.
 *
 * @param game_over Game-over state to mutate.
 * @param hovered_option Option currently under the cursor.
 * @param clicked Whether the user clicked this frame.
 * @return Action requested by the game-over screen.
 */
GameOverAction game_over_update(game_over_t *game_over, GameOverOption hovered_option, bool clicked);

#endif /* _GAME_OVER_H_ */
