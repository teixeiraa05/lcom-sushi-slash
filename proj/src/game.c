#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../include/game/game.h"
#include "../include/macros/mouse.h"
#include "../include/ui/ui.h"

// 1. Sushi management

static void gameplay_remove_sushi(GameInfo *game, uint8_t index) {
    if (game == NULL || index >= game->singleplayer.num_active_sushi) return;

    for (uint8_t i = index; i + 1 < game->singleplayer.num_active_sushi; i++) {
        game->singleplayer.active_sushi[i] = game->singleplayer.active_sushi[i + 1];
    }
    game->singleplayer.num_active_sushi--;
}

static void gameplay_remove_fragment(GameInfo *game, uint8_t index) {
    if (game == NULL || index >= game->singleplayer.num_active_fragments) return;

    for (uint8_t i = index; i + 1 < game->singleplayer.num_active_fragments; i++) {
        game->singleplayer.active_fragments[i] = game->singleplayer.active_fragments[i + 1];
    }
    game->singleplayer.num_active_fragments--;
}

// 2. Wave and difficulty handling

static difficulty_t gameplay_difficulty_for_score(uint32_t score) {
    if (score >= GAME_HARD_SCORE) return HARD;
    if (score >= GAME_NORMAL_SCORE) return NORMAL;
    return EASY;
}

static void gameplay_apply_difficulty(GameInfo *game, difficulty_t difficulty) {
    if (game == NULL) return;

    game->singleplayer.current_difficulty = difficulty;
    waves_apply_difficulty(game->singleplayer.waves, WAVE_PATTERN_COUNT, difficulty);
}

static void gameplay_load_waves(GameInfo *game) {
    if (game == NULL) return;

    for (uint8_t i = 0; i < WAVE_PATTERN_COUNT; i++) {
        const wave_t *wave = waves_get_predefined((wave_pattern_t)i);
        if (wave != NULL) {
            game->singleplayer.waves[i] = *wave;
        }
    }

    gameplay_apply_difficulty(game, EASY);
}

static void gameplay_update_difficulty(GameInfo *game) {
    if (game == NULL) return;

    difficulty_t difficulty = gameplay_difficulty_for_score(game->score);
    if (difficulty != game->singleplayer.current_difficulty) {
        gameplay_apply_difficulty(game, difficulty);
    }
}

static void gameplay_get_difficulty_physics(const GameInfo *game, int *speed, int *gravity) {
    if (speed == NULL || gravity == NULL) return;

    difficulty_t difficulty = game == NULL ? EASY : game->singleplayer.current_difficulty;
    const motion_profile_t *motion = waves_get_difficulty_motion(difficulty);
    int min_speed = PHYSICS_TO_PIXELS(-motion->max_vy);
    int max_speed = PHYSICS_TO_PIXELS(-motion->min_vy);

    if (max_speed < min_speed) {
        max_speed = min_speed;
    }

    *speed = min_speed + rand() % (max_speed - min_speed + 1);
    *gravity = motion->gravity;
}

static void gameplay_add_item(GameInfo *game, sushi_t item) {
    if (game == NULL || item.xpm == NULL) {
        sushi_destroy(&item);
        return;
    }

    if (game->singleplayer.num_active_sushi >= MAX_ACTIVE_SUSHI) {
        sushi_destroy(&item);
        return;
    }

    game->singleplayer.active_sushi[game->singleplayer.num_active_sushi++] = item;
}

static void gameplay_spawn_wave_item(GameInfo *game, const wave_spawn_request_t *request, int speed, int gravity) {
    if (game == NULL || request == NULL) return;

    switch (request->kind) {
        case WAVE_SPAWN_REGULAR: {
            sushi_t sushi = create_random_sushi(speed, gravity);
            sushi.score_value = (int)request->score_value;
            gameplay_add_item(game, sushi);
            break;
        }
        case WAVE_SPAWN_BOMB:
            gameplay_add_item(game, create_random_bomb(speed, gravity));
            break;
        default:
            break;
    }
}

static void gameplay_handle_wave_spawn(GameInfo *game, const wave_spawn_request_t *request) {
    if (game == NULL || request == NULL) return;

    for (uint8_t i = 0; i < request->amount; i++) {
        int speed = 0;
        int gravity = 0;
        gameplay_get_difficulty_physics(game, &speed, &gravity);

        gameplay_spawn_wave_item(game, request, speed, gravity);
    }
}
// 3. Gameplay state

static bool sushi_missed_lower_bound(const sushi_t *sushi) {
    return sushi != NULL &&
           sushi->xpm != NULL &&
           sushi_screen_y(sushi) > SCREEN_HEIGHT + SUSHI_MISS_Y_OFFSET &&
           sushi->vy > 0;
}

void gameplay_clear(GameInfo *game) {
    if (game == NULL) return;

    for (uint8_t i = 0; i < game->singleplayer.num_active_sushi; i++) {
        sushi_destroy(&game->singleplayer.active_sushi[i]);
    }

    for (uint8_t i = 0; i < game->singleplayer.num_active_fragments; i++) {
        sushi_fragment_destroy(&game->singleplayer.active_fragments[i]);
    }

    game->singleplayer.num_active_sushi = 0;
    game->singleplayer.num_active_fragments = 0;
}

static void gameplay_end_game(GameInfo *game) {
    if (game == NULL) return;

    uint32_t final_score = game->score;
    game->lives = 0;
    gameplay_clear(game);
    game->state = GAME_OVER;
    game_over_init(&game->data.game_over, final_score);
}

static void gameplay_handle_missed_item(GameInfo *game, uint8_t index) {
    if (game == NULL || index >= game->singleplayer.num_active_sushi) return;

    sushi_kind_t kind = game->singleplayer.active_sushi[index].kind;
    sushi_destroy(&game->singleplayer.active_sushi[index]);
    gameplay_remove_sushi(game, index);

    if (kind == SUSHI_BOMB) {
        return;
    }

    if (game->lives > 0) {
        game->lives--;
    }

    if (game->lives == 0) {
        gameplay_end_game(game);
    }
}

void gameplay_start(GameInfo *game) {
    if (game == NULL) return;

    memset(&game->singleplayer, 0, sizeof(game->singleplayer));
    gameplay_load_waves(game);
    waves_init(&game->singleplayer.wave_runtime, 0);
    game->score = 0;
    game->lives = LIVES;
}

void gameplay_update(GameInfo *game, int current_tick, int *last_spawn_tick) {
    if (game == NULL || last_spawn_tick == NULL) return;

    gameplay_update_difficulty(game);

    wave_spawn_request_t request;
    bool has_request;
    wave_status_t wave_status;
    do {
        wave_status = waves_update(&game->singleplayer.wave_runtime,
                                   game->singleplayer.waves,
                                   WAVE_PATTERN_COUNT,
                                   game->singleplayer.current_difficulty,
                                   (uint32_t)current_tick,
                                   &request,
                                   &has_request);
        if (has_request) {
            gameplay_handle_wave_spawn(game, &request);
            *last_spawn_tick = current_tick;
        }
    }
    while (has_request && wave_status == WAVE_STATUS_RUNNING);

    uint8_t i = 0;
    while (i < game->singleplayer.num_active_sushi) {
        update_sushi(&game->singleplayer.active_sushi[i]);

        if (sushi_missed_lower_bound(&game->singleplayer.active_sushi[i])) {
            gameplay_handle_missed_item(game, i);
            if (game->state != GAME) return;
        }
        else if (game->singleplayer.active_sushi[i].xpm == NULL) {
            gameplay_remove_sushi(game, i);
        }
        else {
            i++;
        }
    }

    i = 0;
    while (i < game->singleplayer.num_active_fragments) {
        update_sushi_fragment(&game->singleplayer.active_fragments[i]);
        if (game->singleplayer.active_fragments[i].xpm == NULL) {
            gameplay_remove_fragment(game, i);
        }
        else {
            i++;
        }
    }
}

void gameplay_draw(const GameInfo *game) {
    if (game == NULL) return;

    for (uint8_t i = 0; i < game->singleplayer.num_active_sushi; i++) {
        const sushi_t *sushi = &game->singleplayer.active_sushi[i];
        int x = sushi_screen_x(sushi);
        int y = sushi_screen_y(sushi);
        if (sushi->xpm != NULL && x >= 0 && y >= 0) {
            ui_draw_sprite(sushi->xpm, (uint16_t)x, (uint16_t)y);
        }
    }

    for (uint8_t i = 0; i < game->singleplayer.num_active_fragments; i++) {
        const sushi_fragment_t *fragment = &game->singleplayer.active_fragments[i];
        int x = sushi_fragment_screen_x(fragment);
        int y = sushi_fragment_screen_y(fragment);
        if (fragment->xpm != NULL && x >= 0 && y >= 0) {
            ui_draw_sprite(fragment->xpm, (uint16_t)x, (uint16_t)y);
        }
    }
}

// 4. Cutting

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
    while (i < game->singleplayer.num_active_sushi) {
        sushi_t *sushi = &game->singleplayer.active_sushi[i];

        if (!sushi_segment_intersects(sushi,
                                      prev_cut_x,
                                      prev_cut_y,
                                      cut_x,
                                      cut_y,
                                      MOUSE_SLASH_HIT_RADIUS)) {
            i++;
            continue;
        }

        if (sushi->kind == SUSHI_BOMB) {
            gameplay_end_game(game);
            return;
        }

        if (sushi->kind == SUSHI_REGULAR) {
            if (game->singleplayer.num_active_fragments + 2 > MAX_ACTIVE_FRAGMENTS) {
                return;
            }

            sushi_fragment_t left_fragment;
            sushi_fragment_t right_fragment;
            if (cut_sushi(sushi, &left_fragment, &right_fragment) != 0) {
                i++;
                continue;
            }

            game->score += sushi->score_value;
            game->singleplayer.active_fragments[game->singleplayer.num_active_fragments++] = left_fragment;
            game->singleplayer.active_fragments[game->singleplayer.num_active_fragments++] = right_fragment;
            gameplay_remove_sushi(game, i);
        }
        else {
            i++;
        }
    }
}
