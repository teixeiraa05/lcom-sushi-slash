#ifndef __KEYBOARD_PROJ_H
#define __KEYBOARD_PROJ_H

#include <stdint.h>
#include <stdbool.h>

#include "../../lab3/kbc.h"
#include "../../lab3/i8042.h"

/**
 * @brief Initializes the keyboard driver by subscribing to its interrupts.
 * 
 * @param bit_no Pointer to store the IRQ bitmask hook ID.
 * @return 0 on success, non-zero on failure.
 */
int keyboard_init(uint8_t *bit_no);

/**
 * @brief Unsubscribes from keyboard interrupts.
 * 
 * @return 0 on success, non-zero on failure.
 */
int keyboard_cleanup(void);

/**
 * @brief Project-level keyboard interrupt handler.
 * 
 * Reads KBC status and output registers, processes two-byte extended (E0) scancodes,
 * and updates the internal last-received key state.
 */
void keyboard_ih(void);

/**
 * @brief Checks if a new key has been pressed and retrieves its scancode.
 * 
 * Once retrieved, the new-key flag is reset.
 * 
 * @param scancode Pointer to store the retrieved scancode if a new key exists.
 * @return true if a new key scancode was available and retrieved, false otherwise.
 */
bool keyboard_get_scancode(uint8_t *scancode);

/**
 * @brief Translates a keyboard scancode to its printable ASCII character.
 * 
 * Highly useful for validating character sequences typed during boss battles.
 * 
 * @param scancode The raw keyboard make code.
 * @return The corresponding uppercase character (A-Z, etc.), or 0 if unprintable or unsupported.
 */
char keyboard_scancode_to_ascii(uint8_t scancode);

#endif /* __KEYBOARD_PROJ_H */
