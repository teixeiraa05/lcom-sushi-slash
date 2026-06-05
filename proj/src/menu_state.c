#include "../include/game/menu_state.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void menu_init(menu_t *menu) {
    if (menu == NULL) return;

    menu->hovered_option = MENU_OPTION_NONE;
    menu->entering_seed = false;
    menu->seed_str[0] = '\0';
    menu->seed_val = 0;
}

MenuAction menu_update(menu_t *menu, MenuOption hovered_option, bool clicked) {
    if (menu == NULL) return MENU_ACTION_NONE;

    menu->hovered_option = hovered_option;
    if (!clicked) return MENU_ACTION_NONE;

    switch (hovered_option) {
        case MENU_OPTION_SINGLEPLAYER:
            menu->entering_seed = false;
            return MENU_ACTION_START_SINGLEPLAYER;
        case MENU_OPTION_SEED:
            menu->entering_seed = !menu->entering_seed;
            return MENU_ACTION_ENTER_SEED;
        case MENU_OPTION_INSTRUCTIONS:
            menu->entering_seed = false;
            return MENU_ACTION_SHOW_INSTRUCTIONS;
        case MENU_OPTION_EXIT:
            menu->entering_seed = false;
            return MENU_ACTION_EXIT;
        case MENU_OPTION_NONE:
        default:
            menu->entering_seed = false;
            return MENU_ACTION_NONE;
    }
}

void menu_handle_key(menu_t *menu, char c) {
    if (menu == NULL || !menu->entering_seed) return;

    size_t len = strlen(menu->seed_str);
    if (c >= '0' && c <= '9' && len < 9) {
        menu->seed_str[len] = c;
        menu->seed_str[len + 1] = '\0';
        menu->seed_val = (uint32_t)strtoul(menu->seed_str, NULL, 10);
    }
    else if (c == '\b' && len > 0) {
        menu->seed_str[len - 1] = '\0';
        menu->seed_val = (len - 1 > 0) ? (uint32_t)strtoul(menu->seed_str, NULL, 10) : 0;
    }
    else if (c == '\n' || c == 27) {
        menu->entering_seed = false;
    }
}
