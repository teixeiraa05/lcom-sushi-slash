#ifndef __ENTITIES_H
#define __ENTITIES_H

#include "../../lab5/video_gr.h"

typedef struct bomb {
    int x, y, vx, vy;
    xpm_map_t xpm;
} bomb_t;

typedef struct sushi {
    int x, y, vx, vy;
    xpm_map_t xpm;
} sushi_t;

// We need to split the sushi into two parts, so call a function that eliminates the sushi and creates two splitted_sushi_t, 
// with the same x and y as the original sushi, but with different vx and vy
typedef struct splitted_sushi {
    int x, y, vx, vy;
    xpm_map_t xpm;
} splitted_sushi_t;

#endif /* __ENTITIES_H */
