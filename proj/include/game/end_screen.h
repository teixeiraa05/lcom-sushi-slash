#ifndef __END_SCREEN_H
#define __END_SCREEN_H

#include "../../lab5/video_gr.h"
#include "sprite.h"

typedef struct end_screen {
    sprite_t xpm; //win/loss xpm
    sprite_t mascot; //mascot xpm
    
    sprite_t** score; //to load the font for the score  
} end_screen_t;

#endif /* __END_SCREEN_H */
