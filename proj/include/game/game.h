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

/**
 * @file game.h
 * @brief Declares the public game state and gameplay lifecycle API.
 */

/** @brief Initial number of player lives in a gameplay session. */
#define LIVES 3

/** @brief Maximum number of active gameplay items on screen. */
#define MAX_ACTIVE_SUSHI 8

/** @brief Maximum number of active cut fragments on screen. */
#define MAX_ACTIVE_FRAGMENTS 16

/** @brief Score threshold at which normal difficulty starts. */
#define GAME_NORMAL_SCORE 15

/** @brief Score threshold at which hard difficulty starts. */
#define GAME_HARD_SCORE 50

/**
 * @brief High-level state of the application.
 */
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

/**
 * @brief Device categories used by the game.
 */
typedef enum {
  KEYBOARD,
  MOUSE,
  TIMER,
  RTC, 
  GRAPHICS,
  SERIALPORT
} Device;

/**
 * @brief Runtime data for a singleplayer gameplay session.
 */
typedef struct {
    /** @brief Active regular sushi and bomb items. */
    sushi_t active_sushi[MAX_ACTIVE_SUSHI];

    /** @brief Active fragments created by cut regular sushi. */
    sushi_fragment_t active_fragments[MAX_ACTIVE_FRAGMENTS];

    /** @brief Mutable wave sequence used by the current difficulty. */
    wave_t waves[WAVE_PATTERN_COUNT];

    /** @brief Wave scheduler runtime state. */
    wave_runtime_t wave_runtime;

    /** @brief Current difficulty level. */
    difficulty_t current_difficulty;

    /** @brief Number of active entries in active_sushi. */
    uint8_t num_active_sushi;

    /** @brief Number of active entries in active_fragments. */
    uint8_t num_active_fragments;
} gameplay_data_t;

/**
 * @brief Main game state container shared by menus and gameplay modes.
 */
typedef struct {
    /** @brief Current high-level application state. */
    GameState state;

    /** @brief Current player score. */
    uint32_t score;

    /** @brief Current remaining player lives. */
    uint8_t lives;

    /** @brief Gameplay background sprite. */
    sprite_t background;

    /** @brief Singleplayer gameplay runtime data. */
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

/**
 * @brief Run the main game loop.
 *
 * @return 0 on successful exit, non-zero on failure.
 */
int game_loop();

/**
 * @brief Destroy active gameplay objects and reset active object counts.
 *
 * @param game Game state to clear.
 */
void gameplay_clear(GameInfo *game);

/**
 * @brief Initialize a new singleplayer gameplay session.
 *
 * @param game Game state to initialize.
 */
void gameplay_start(GameInfo *game);

/**
 * @brief Advance singleplayer gameplay by one tick.
 *
 * Handles wave spawns, item physics, missed items, fragments, difficulty
 * changes, and game-over transitions.
 *
 * @param game Game state to update.
 * @param current_tick Current gameplay/session tick.
 * @param last_spawn_tick Output storage updated when a wave emits a spawn.
 */
void gameplay_update(GameInfo *game, int current_tick, int *last_spawn_tick);

/**
 * @brief Draw active gameplay items and fragments.
 *
 * @param game Game state to draw.
 */
void gameplay_draw(const GameInfo *game);

/**
 * @brief Process the current mouse slash against active gameplay items.
 *
 * Regular sushi is cut into fragments and awards score. Bombs immediately end
 * the gameplay session when hit.
 *
 * @param game Game state to mutate.
 * @param mouse_state Current mouse gameplay input state.
 */
void gameplay_handle_cut(GameInfo *game, MouseGameState *mouse_state);

#endif /* __GAME_H */
