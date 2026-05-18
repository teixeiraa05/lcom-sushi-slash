#ifndef __COMBO_H
#define __COMBO_H

#include "sprite.h"

typedef void* queue;

typedef struct combo {
    queue combo; // A queue of the required moves for the combo

    sprite_t current_letter; // The current letter to be displayed on the screen

    uint16_t x, y; // Where letters appear on the screen
    
} combo_t;

#endif /* __COMBO_H */
