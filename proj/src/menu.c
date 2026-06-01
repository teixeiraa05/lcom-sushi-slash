#include "../include/game/menu.h"
#include <string.h>

void menu_init(menu_t *menu) {
    if (menu == NULL) return;

    menu->hovered_option = MENU_OPTION_NONE;
    menu->selected_difficulty = EASY;
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

void pause_menu_init(pause_menu_t *menu) {
    if (menu == NULL) return;

    menu->hovered_option = PAUSE_OPTION_NONE;
}

PauseAction pause_menu_update(pause_menu_t *menu, PauseOption hovered_option, bool clicked) {
    if (menu == NULL) return PAUSE_ACTION_NONE;

    menu->hovered_option = hovered_option;
    if (!clicked) return PAUSE_ACTION_NONE;

    switch (hovered_option) {
        case PAUSE_OPTION_RESUME:
            return PAUSE_ACTION_RESUME;
        case PAUSE_OPTION_MAIN_MENU:
            return PAUSE_ACTION_MAIN_MENU;
        case PAUSE_OPTION_EXIT:
            return PAUSE_ACTION_EXIT;
        case PAUSE_OPTION_NONE:
        default:
            return PAUSE_ACTION_NONE;
    }
}
