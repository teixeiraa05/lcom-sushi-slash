#ifndef __COMBO_H
#define __COMBO_H

#include "sprite.h"
#include "queue.h"

/**
 * @file combo.h
 * @brief Declares boss combo prompt runtime data.
 */

/**
 * @brief Key-sequence prompt used by boss encounters.
 */
typedef struct combo {
    /** @brief Queue of required key inputs for the combo. */
    Queue combo;

    /** @brief Sprite for the current letter displayed on screen. */
    sprite_t current_letter;

    /** @brief Horizontal coordinate where letters appear on screen. */
    uint16_t x;

    /** @brief Vertical coordinate where letters appear on screen. */
    uint16_t y;
    
} combo_t;

#endif /* __COMBO_H */
