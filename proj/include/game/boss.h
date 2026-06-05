#ifndef __BOSS_H
#define __BOSS_H

#include <lcom/lcf.h> 

#include "sprite.h"
#include "sushi.h"
#include "combo.h"

/**
 * @file boss.h
 * @brief Declares Boss Sushi runtime data.
 */

/**
 * @brief Runtime state for a boss encounter.
 */
typedef struct boss {
    /** @brief Physical boss sushi object shown on screen. */
    sushi_t sushi;

    /** @brief Required key sequence for defeating the boss. */
    combo_t combo;

    /** @brief Time remaining until the boss fight ends, in ticks. */
    uint32_t countdown;
} boss_t;

/**
 * @brief Initializes the boss with the given parameters.
 *
 * @param combo_combo Combo structure to track the boss's combo state.
 * @param countdown_ticks Time until the end of the boss fight in ticks.
 */
boss_t boss_init(combo_t combo, uint32_t countdown_ticks);

/**
 * @brief Updates the boss's state, if the input matches the current combo front letter.
 *
 * @param boss Pointer to the boss to update.
 * @param input The character input to verify.
 * @return 0 if the input matches the current combo front letter, 1 otherwise.
 */
int boss_verify_combo(boss_t* boss, char input);

/**
 * @brief Updates the boss's state, such as moving the sushi.
 *
 * @param boss Pointer to the boss to update.
 */
void boss_update(boss_t* boss);

/**
 * @brief Destroys the boss's resources, such as the sushi sprite.
 *
 * @param boss Pointer to the boss to destroy.
 */
void boss_destroy(boss_t* boss);



#endif /* __BOSS_H */
