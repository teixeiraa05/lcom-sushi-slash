#include <lcom/lcf.h>
#include <string.h>

#include "../include/game/game.h"
#include "../include/macros/mouse.h"
#include "../include/ui/ui.h"


void gameplay_clear(GameInfo *game) {
    if (game == NULL) return;

    for (uint8_t i = 0; i < game->data.singleplayer.num_active_sushi; i++) {
        sushi_destroy(&game->data.singleplayer.active_sushi[i]);
    }

    for (uint8_t i = 0; i < game->data.singleplayer.num_active_fragments; i++) {
        sushi_fragment_destroy(&game->data.singleplayer.active_fragments[i]);
    }

    game->data.singleplayer.num_active_sushi = 0;
    game->data.singleplayer.num_active_fragments = 0;
}

void gameplay_start(GameInfo *game) {
    if (game == NULL) return;

    memset(&game->data.singleplayer, 0, sizeof(game->data.singleplayer));
    game->score = 0;
    game->lives = LIVES;
}

static void gameplay_remove_sushi(GameInfo *game, uint8_t index) {
    if (game == NULL || index >= game->data.singleplayer.num_active_sushi) return;

    for (uint8_t i = index; i + 1 < game->data.singleplayer.num_active_sushi; i++) {
        game->data.singleplayer.active_sushi[i] = game->data.singleplayer.active_sushi[i + 1];
    }
    game->data.singleplayer.num_active_sushi--;
}

static void gameplay_remove_fragment(GameInfo *game, uint8_t index) {
    if (game == NULL || index >= game->data.singleplayer.num_active_fragments) return;

    for (uint8_t i = index; i + 1 < game->data.singleplayer.num_active_fragments; i++) {
        game->data.singleplayer.active_fragments[i] = game->data.singleplayer.active_fragments[i + 1];
    }
    game->data.singleplayer.num_active_fragments--;
}

static void gameplay_spawn_sushi(GameInfo *game) {
    if (game == NULL || game->data.singleplayer.num_active_sushi >= MAX_ACTIVE_SUSHI) return;

    int speed = GAME_SPEED_1;
    int gravity = GAME_GRAVITY_1;
    if (game->score >= GAME_SPEED_5_SCORE) {
        speed = GAME_SPEED_5;
        gravity = GAME_GRAVITY_5;
    }
    else if (game->score >= GAME_SPEED_4_SCORE) {
        speed = GAME_SPEED_4;
        gravity = GAME_GRAVITY_4;
    }
    else if (game->score >= GAME_SPEED_3_SCORE) {
        speed = GAME_SPEED_3;
        gravity = GAME_GRAVITY_3;
    }
    else if (game->score >= GAME_SPEED_2_SCORE) {
        speed = GAME_SPEED_2;
        gravity = GAME_GRAVITY_2;
    }

    sushi_t sushi = create_random_sushi(speed, gravity);
    if (sushi.xpm == NULL) return;

    game->data.singleplayer.active_sushi[game->data.singleplayer.num_active_sushi++] = sushi;
}

void gameplay_update(GameInfo *game, int current_tick, int *last_spawn_tick) {
    if (game == NULL || last_spawn_tick == NULL) return;

    if (current_tick - *last_spawn_tick >= TEST_SUSHI_SPAWN_TICKS) {
        gameplay_spawn_sushi(game);
        *last_spawn_tick = current_tick;
    }

    uint8_t i = 0;
    while (i < game->data.singleplayer.num_active_sushi) {
        update_sushi(&game->data.singleplayer.active_sushi[i]);
        if (game->data.singleplayer.active_sushi[i].xpm == NULL) {
            gameplay_remove_sushi(game, i);
        }
        else {
            i++;
        }
    }

    i = 0;
    while (i < game->data.singleplayer.num_active_fragments) {
        update_sushi_fragment(&game->data.singleplayer.active_fragments[i]);
        if (game->data.singleplayer.active_fragments[i].xpm == NULL) {
            gameplay_remove_fragment(game, i);
        }
        else {
            i++;
        }
    }
}

void gameplay_draw(const GameInfo *game) {
    if (game == NULL) return;

    for (uint8_t i = 0; i < game->data.singleplayer.num_active_sushi; i++) {
        const sushi_t *sushi = &game->data.singleplayer.active_sushi[i];
        int x = sushi_screen_x(sushi);
        int y = sushi_screen_y(sushi);
        if (sushi->xpm != NULL && x >= 0 && y >= 0) {
            ui_draw_sprite(sushi->xpm, (uint16_t)x, (uint16_t)y);
        }
    }

    for (uint8_t i = 0; i < game->data.singleplayer.num_active_fragments; i++) {
        const sushi_fragment_t *fragment = &game->data.singleplayer.active_fragments[i];
        int x = sushi_fragment_screen_x(fragment);
        int y = sushi_fragment_screen_y(fragment);
        if (fragment->xpm != NULL && x >= 0 && y >= 0) {
            ui_draw_sprite(fragment->xpm, (uint16_t)x, (uint16_t)y);
        }
    }
}

void gameplay_handle_cut(GameInfo *game, MouseGameState *mouse_state) {
    if (game == NULL || mouse_state == NULL) return;

    if (!mouse_state->lb_pressed || !mouse_state->is_slashing) {
        mouse_state->prev_x = mouse_state->x;
        mouse_state->prev_y = mouse_state->y;
        return;
    }

    int cut_x = mouse_state->x + MOUSE_CURSOR_CENTER_OFFSET_X;
    int cut_y = mouse_state->y + MOUSE_CURSOR_CENTER_OFFSET_Y;
    int prev_cut_x = mouse_state->prev_x + MOUSE_CURSOR_CENTER_OFFSET_X;
    int prev_cut_y = mouse_state->prev_y + MOUSE_CURSOR_CENTER_OFFSET_Y;

    mouse_state->prev_x = mouse_state->x;
    mouse_state->prev_y = mouse_state->y;

    uint8_t i = 0;
    while (i < game->data.singleplayer.num_active_sushi) {
        if (game->data.singleplayer.num_active_fragments + 2 > MAX_ACTIVE_FRAGMENTS) return;

        sushi_fragment_t left_fragment;
        sushi_fragment_t right_fragment;
        if (find_cut_sushi_segment_area(&game->data.singleplayer.active_sushi[i],
                                        prev_cut_x,
                                        prev_cut_y,
                                        cut_x,
                                        cut_y,
                                        MOUSE_SLASH_HIT_RADIUS,
                                        &left_fragment,
                                        &right_fragment) == 0) {
            game->score += game->data.singleplayer.active_sushi[i].score_value;
            game->data.singleplayer.active_fragments[game->data.singleplayer.num_active_fragments++] = left_fragment;
            game->data.singleplayer.active_fragments[game->data.singleplayer.num_active_fragments++] = right_fragment;
            gameplay_remove_sushi(game, i);
        }
        else {
            i++;
        }
    }
}
