#ifndef __MENU_H
#define __MENU_H

#include "sprite.h"
#include "button.h"

typedef struct menu {
    sprite_t background;

    button_t singleplayer_button;
    button_t multiplayer_button;
    button_t instructions_button;
    button_t exit_button;
} menu_t;

typedef struct {
    sprite_t background;

    button_t resume_button;
    button_t exit_button;
} pause_menu_t;

#endif /* __MENU_H */
