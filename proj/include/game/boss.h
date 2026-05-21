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

#endif /* __BOSS_H */
