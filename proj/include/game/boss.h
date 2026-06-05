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

#endif /* __BOSS_H */
