#ifndef __GAME_H
#define __GAME_H

#include "boss.h"
#include "entities.h"
#include "difficulty.h"

typedef struct game {
    boss_t boss[2];
    sushi_t sushi[15];
    difficulty_t difficulty;
    int score;
    int multiplier; // For the score, increases with combos, resets when the player misses a sushi or boss combo
} game_t;

#endif /* __GAME_H */
