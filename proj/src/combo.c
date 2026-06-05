#include "../include/game/combo.h"
#include "../assets/keyboard/KeyA.xpm"
#include "../assets/keyboard/KeyB.xpm"
#include "../assets/keyboard/KeyC.xpm"
#include "../assets/keyboard/KeyD.xpm"
#include "../assets/keyboard/KeyE.xpm"
#include "../assets/keyboard/KeyF.xpm"
#include "../assets/keyboard/KeyG.xpm"
#include "../assets/keyboard/KeyH.xpm"
#include "../assets/keyboard/KeyI.xpm"
#include "../assets/keyboard/KeyJ.xpm"
#include "../assets/keyboard/KeyK.xpm"
#include "../assets/keyboard/KeyL.xpm"
#include "../assets/keyboard/KeyM.xpm"
#include "../assets/keyboard/KeyN.xpm"
#include "../assets/keyboard/KeyO.xpm"
#include "../assets/keyboard/KeyP.xpm"
#include "../assets/keyboard/KeyQ.xpm"
#include "../assets/keyboard/KeyR.xpm"
#include "../assets/keyboard/KeyS.xpm"
#include "../assets/keyboard/KeyT.xpm"
#include "../assets/keyboard/KeyU.xpm"
#include "../assets/keyboard/KeyV.xpm"
#include "../assets/keyboard/KeyW.xpm"
#include "../assets/keyboard/KeyX.xpm"
#include "../assets/keyboard/KeyY.xpm"
#include "../assets/keyboard/KeyZ.xpm"

// Mapping table from character to XPM image
typedef struct {
    char character;
    xpm_map_t xpm;
} char_to_xpm_t;

static const char_to_xpm_t keyboard_map[52] = {
    {'A', (xpm_map_t)KeyA_xpm}, {'a', (xpm_map_t)KeyA_xpm},
    {'B', (xpm_map_t)KeyB_xpm}, {'b', (xpm_map_t)KeyB_xpm},
    {'C', (xpm_map_t)KeyC_xpm}, {'c', (xpm_map_t)KeyC_xpm},
    {'D', (xpm_map_t)KeyD_xpm}, {'d', (xpm_map_t)KeyD_xpm},
    {'E', (xpm_map_t)KeyE_xpm}, {'e', (xpm_map_t)KeyE_xpm},
    {'F', (xpm_map_t)KeyF_xpm}, {'f', (xpm_map_t)KeyF_xpm},
    {'G', (xpm_map_t)KeyG_xpm}, {'g', (xpm_map_t)KeyG_xpm},
    {'H', (xpm_map_t)KeyH_xpm}, {'h', (xpm_map_t)KeyH_xpm},
    {'I', (xpm_map_t)KeyI_xpm}, {'i', (xpm_map_t)KeyI_xpm},
    {'J', (xpm_map_t)KeyJ_xpm}, {'j', (xpm_map_t)KeyJ_xpm},
    {'K', (xpm_map_t)KeyK_xpm}, {'k', (xpm_map_t)KeyK_xpm},
    {'L', (xpm_map_t)KeyL_xpm}, {'l', (xpm_map_t)KeyL_xpm},
    {'M', (xpm_map_t)KeyM_xpm}, {'m', (xpm_map_t)KeyM_xpm},
    {'N', (xpm_map_t)KeyN_xpm}, {'n', (xpm_map_t)KeyN_xpm},
    {'O', (xpm_map_t)KeyO_xpm}, {'o', (xpm_map_t)KeyO_xpm},
    {'P', (xpm_map_t)KeyP_xpm}, {'p', (xpm_map_t)KeyP_xpm},
    {'Q', (xpm_map_t)KeyQ_xpm}, {'q', (xpm_map_t)KeyQ_xpm},
    {'R', (xpm_map_t)KeyR_xpm}, {'r', (xpm_map_t)KeyR_xpm},
    {'S', (xpm_map_t)KeyS_xpm}, {'s', (xpm_map_t)KeyS_xpm},
    {'T', (xpm_map_t)KeyT_xpm}, {'t', (xpm_map_t)KeyT_xpm},
    {'U', (xpm_map_t)KeyU_xpm}, {'u', (xpm_map_t)KeyU_xpm},
    {'V', (xpm_map_t)KeyV_xpm}, {'v', (xpm_map_t)KeyV_xpm},
    {'W', (xpm_map_t)KeyW_xpm}, {'w', (xpm_map_t)KeyW_xpm},
    {'X', (xpm_map_t)KeyX_xpm}, {'x', (xpm_map_t)KeyX_xpm},
    {'Y', (xpm_map_t)KeyY_xpm}, {'y', (xpm_map_t)KeyY_xpm},
    {'Z', (xpm_map_t)KeyZ_xpm}, {'z', (xpm_map_t)KeyZ_xpm},
};

#define KEYBOARD_MAP_SIZE (sizeof(keyboard_map) / sizeof(char_to_xpm_t))

// Function to get XPM image from a character
xpm_map_t char_to_keyboard_xpm(char c) {
    for (size_t i = 0; i < KEYBOARD_MAP_SIZE; i++) {
        if (keyboard_map[i].character == c) {
            return keyboard_map[i].xpm;
        }
    }
    return NULL; // Character not found
}

combo_t combo_init(const char* sequence, uint16_t x, uint16_t y) {
    combo_t combo;
    Queue q;
    queue_init(&q);
    for (size_t i = 0; i < strlen(sequence); i++) {
        queue_push(&q, sequence[i]);
    }
    combo.combo = q;

    char current_letter;
    queue_top(&combo.combo, &current_letter);
    if (current_letter == 0) {
        current_letter = 'A'; // Default to 'A' for an empty combo
    }
    xpm_map_t xpm = char_to_keyboard_xpm(current_letter);
    sprite_load(&combo.current_letter, xpm);
    return combo;
}

combo_t create_random_combo(uint8_t length, uint16_t x, uint16_t y) {
    combo_t combo;
    char* sequence = malloc(length + 1);
    for (uint8_t i = 0; i < length; i++) {
        sequence[i] = 'A' + (rand() % 26); // Random uppercase letter
    }
    sequence[length] = '\0'; // Null-terminate the string
    
    combo = combo_init(sequence, x, y);
    free(sequence);
    return combo;
}