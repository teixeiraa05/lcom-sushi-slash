#ifndef __BOSS_H
#define __BOSS_H

#include "../../lab5/video_gr.h"
#include "combo.h"

typedef struct boss {
    int x, y;
    xpm_map_t xpm;
    combo_t combo; // The combo that kills the boss, could be better to use a queue or something else
} boss_t;

#endif /* __BOSS_H */
