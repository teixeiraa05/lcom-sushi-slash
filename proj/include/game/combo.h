#ifndef __COMBO_H
#define __COMBO_H

#include "sprite.h"
#include "queue.h"
#include <lcom/lcf.h>

typedef struct combo {
    Queue combo; // A queue of the required moves for the combo

    sprite_t current_letter; // The current letter to be displayed on the screen

    uint16_t x, y; // Where letters appear on the screen
    
} combo_t;

combo_t combo_init(const char* sequence, uint16_t x, uint16_t y);

combo_t create_random_combo(uint8_t length, uint16_t x, uint16_t y);

// Function to convert a character to its keyboard XPM image
xpm_map_t char_to_keyboard_xpm(char c);

#endif /* __COMBO_H */
