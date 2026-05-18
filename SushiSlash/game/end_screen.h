#ifndef __END_SCREEN_H
#define __END_SCREEN_H

#include "../../lab5/video_gr.h"

typedef struct end_screen {
    xpm_map_t xpm; // Could be a different xpm for win and lose
    int score;
} end_screen_t;

#endif /* __END_SCREEN_H */
