#include "../include/game/menu_state.h"
#include <stdio.h>

void menu_init(menu_t *menu) {
    if (menu == NULL) return;

    menu->hovered_option = MENU_OPTION_NONE;
}

MenuAction menu_update(menu_t *menu, MenuOption hovered_option, bool clicked) {
    if (menu == NULL) return MENU_ACTION_NONE;

    menu->hovered_option = hovered_option;
    if (!clicked) return MENU_ACTION_NONE;

    switch (hovered_option) {
        case MENU_OPTION_SINGLEPLAYER:
            return MENU_ACTION_START_SINGLEPLAYER;
        case MENU_OPTION_MULTIPLAYER:
            return MENU_ACTION_START_MULTIPLAYER;
        case MENU_OPTION_INSTRUCTIONS:
            return MENU_ACTION_SHOW_INSTRUCTIONS;
        case MENU_OPTION_EXIT:
            return MENU_ACTION_EXIT;
        case MENU_OPTION_NONE:
        default:
            return MENU_ACTION_NONE;
    }
}
