#ifndef __COMBO_H
#define __COMBO_H

typedef void* queue;

typedef struct combo {
    queue combo; // A queue of the required moves for the combo

    char current_letter; // The current letter the player needs to press (maybe enum?)

    int x, y; // Where letters appear on the screen, if not important, can be removed (fixed position for all combos), 
              // Maybe cool for hard mode, letters appear in random positions?
} combo_t;

#endif /* __COMBO_H */
