#ifndef __STATE_H
#define __STATE_H

#include "menu.h"
#include "game.h"
#include "end_screen.h"

typedef struct state {
    union current {
        menu_t menu;
        game_t game;
        end_screen_t end_screen;
    };
    int time;
} state_t;

#endif /* __STATE_H */
