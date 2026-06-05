#ifndef __BOSS_H
#define __BOSS_H

#include <lcom/lcf.h> 

#include "sprite.h"
#include "sushi.h"
#include "combo.h"

typedef struct boss {
    sushi_t sushi;
    combo_t combo;
    uint32_t countdown; // Time until end of the boss fight in ticks 
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
