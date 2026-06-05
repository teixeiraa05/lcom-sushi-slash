#ifndef __COMBO_H
#define __COMBO_H

#include "sprite.h"
#include "queue.h"
#include <lcom/lcf.h>

typedef struct combo {
    Queue combo; // A queue of the required moves for the combo

    sprite_t current_letter; // The current letter to be displayed on the screen

    uint16_t x, y; // Where letters appear on the screen
    
    uint8_t total_letters; // Total number of letters in the combo
    uint8_t completed; // Number of letters already completed
    
} combo_t;

combo_t combo_init(const char* sequence, uint16_t x, uint16_t y);

combo_t create_random_combo(uint8_t length, uint16_t x, uint16_t y);

// Function to convert a character to its keyboard XPM image
xpm_map_t char_to_keyboard_xpm(char c);

/**
 * @brief Advances the combo to the next letter and updates the display.
 *
 * @param combo Pointer to the combo to advance.
 * @return 0 if advanced successfully, 1 if combo is already complete.
 */
int combo_advance(combo_t* combo);

/**
 * @brief Checks if the combo is complete.
 *
 * @param combo Pointer to the combo to check.
 * @return 1 if combo is complete, 0 otherwise.
 */
int combo_is_complete(const combo_t* combo);

/**
 * @brief Gets the current letter required by the combo.
 *
 * @param combo Pointer to the combo.
 * @return The current letter, or 0 if combo is empty or complete.
 */
char combo_get_current(const combo_t* combo);

#endif /* __COMBO_H */
