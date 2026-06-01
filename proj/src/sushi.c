
#include "../include/game/sushi.h"

#include "../assets/sushi_pieces/Nigiri1.xpm"
#include "../assets/sushi_pieces/Nigiri2.xpm"
#include "../assets/sushi_pieces/Nigiri3.xpm"
#include "../assets/sushi_pieces/Nigiri4.xpm"
#include "../assets/sushi_pieces/Onigiri.xpm"
#include "../assets/sushi_pieces/SushiRoll1.xpm"
#include "../assets/sushi_pieces/SushiRoll2.xpm"
#include "../assets/sushi_pieces/SushiRoll3.xpm"
#include "../assets/sushi_pieces/SushiRoll4.xpm"
#include "../assets/sushi_pieces/SushiRoll5.xpm"
#include "../assets/sushi_pieces/SushiRoll6.xpm"

#include "../assets/sushi_pieces/Nigiri1_left.xpm"
#include "../assets/sushi_pieces/Nigiri2_left.xpm"
#include "../assets/sushi_pieces/Nigiri3_left.xpm"
#include "../assets/sushi_pieces/Nigiri4_left.xpm"
#include "../assets/sushi_pieces/Onigiri_left.xpm"
#include "../assets/sushi_pieces/SushiRoll1_left.xpm"
#include "../assets/sushi_pieces/SushiRoll2_left.xpm"
#include "../assets/sushi_pieces/SushiRoll3_left.xpm"
#include "../assets/sushi_pieces/SushiRoll4_left.xpm"
#include "../assets/sushi_pieces/SushiRoll5_left.xpm"
#include "../assets/sushi_pieces/SushiRoll6_left.xpm"

#include "../assets/sushi_pieces/Nigiri1_right.xpm"
#include "../assets/sushi_pieces/Nigiri2_right.xpm"
#include "../assets/sushi_pieces/Nigiri3_right.xpm"
#include "../assets/sushi_pieces/Nigiri4_right.xpm"
#include "../assets/sushi_pieces/Onigiri_right.xpm"
#include "../assets/sushi_pieces/SushiRoll1_right.xpm"
#include "../assets/sushi_pieces/SushiRoll2_right.xpm"
#include "../assets/sushi_pieces/SushiRoll3_right.xpm"
#include "../assets/sushi_pieces/SushiRoll4_right.xpm"
#include "../assets/sushi_pieces/SushiRoll5_right.xpm"
#include "../assets/sushi_pieces/SushiRoll6_right.xpm"


xpm_map_t sushi_xpms[11] = {
    // Placeholder XPMs for different sushi types
    (xpm_map_t)Nigiri1_xpm,
    (xpm_map_t)Nigiri2_xpm,
    (xpm_map_t)Nigiri3_xpm,
    (xpm_map_t)Nigiri4_xpm,
    (xpm_map_t)Onigiri_xpm,
    (xpm_map_t)SushiRoll1_xpm,
    (xpm_map_t)SushiRoll2_xpm,
    (xpm_map_t)SushiRoll3_xpm,
    (xpm_map_t)SushiRoll4_xpm,
    (xpm_map_t)SushiRoll5_xpm,
    (xpm_map_t)SushiRoll6_xpm
};

xpm_map_t sushi_left_xpms[11] = {
    // Placeholder XPMs for the left fragments of different sushi types
    (xpm_map_t)Nigiri1_left_xpm,
    (xpm_map_t)Nigiri2_left_xpm,
    (xpm_map_t)Nigiri3_left_xpm,
    (xpm_map_t)Nigiri4_left_xpm,
    (xpm_map_t)Onigiri_left_xpm,
    (xpm_map_t)SushiRoll1_left_xpm,
    (xpm_map_t)SushiRoll2_left_xpm,
    (xpm_map_t)SushiRoll3_left_xpm,
    (xpm_map_t)SushiRoll4_left_xpm,
    (xpm_map_t)SushiRoll5_left_xpm,
    (xpm_map_t)SushiRoll6_left_xpm
};

xpm_map_t sushi_right_xpms[11] = {
    // Placeholder XPMs for the right fragments of different sushi types
    (xpm_map_t)Nigiri1_right_xpm,
    (xpm_map_t)Nigiri2_right_xpm,
    (xpm_map_t)Nigiri3_right_xpm,
    (xpm_map_t)Nigiri4_right_xpm,
    (xpm_map_t)Onigiri_right_xpm,
    (xpm_map_t)SushiRoll1_right_xpm,
    (xpm_map_t)SushiRoll2_right_xpm,
    (xpm_map_t)SushiRoll3_right_xpm,
    (xpm_map_t)SushiRoll4_right_xpm,
    (xpm_map_t)SushiRoll5_right_xpm,
    (xpm_map_t)SushiRoll6_right_xpm
};

sushi_t create_sushi(int x, int y, int vx, int vy, sprite_t* xpm, int score_value) {
    sushi_t sushi;
    sushi.x = x;
    sushi.y = y;
    sushi.vx = vx;
    sushi.vy = vy;
    sushi.xpm = xpm;
    sushi.score_value = score_value;
    sushi.type = 0;
    return sushi;
}

sushi_t create_random_sushi() {
    sprite_t *sprite = malloc(sizeof(sprite_t));
    if (sprite == NULL) return create_sushi(0, 0, 0, 0, NULL, 0);

    uint8_t sushi_type = rand() % 11;

    if (sprite_load(sprite, sushi_xpms[sushi_type])) {
        free(sprite);
        return create_sushi(0, 0, 0, 0, NULL, 0);
    }

    sushi_t sushi = create_sushi(rand() % (SCREEN_WIDTH - 100) + 50, 0, rand() % 10 - 5, rand() % 10, sprite, (rand() % 5 + 1) * 10);
    sushi.type = sushi_type;

    return sushi;
}

void update_sushi(sushi_t* sushi) {
    if (sushi == NULL) return;

    sushi->x += sushi->vx;
    sushi->y += sushi->vy;

    sushi->vy -= 1; // Simulate gravity

    // Deactivate if it goes off-screen
    if (sushi->x < 0 || sushi->x > SCREEN_WIDTH ||
        sushi->y < 0 || sushi->y > SCREEN_HEIGHT) {
          sushi_destroy(sushi);
    }
}

void sushi_destroy(sushi_t* sushi) {
    if (sushi != NULL) {
        if (sushi->xpm != NULL) {
            sprite_destroy(sushi->xpm);
            free(sushi->xpm);
            sushi->xpm = NULL;
        }
    }
}

int find_cut_sushi(sushi_t* sushi, int cut_x, int cut_y, sushi_fragment_t* left_fragment, sushi_fragment_t* right_fragment) {
    if (sushi == NULL || sushi->xpm == NULL) return 1;

    // Check if the cut coordinates are within the sushi's bounding box
    if (cut_x >= sushi->x && cut_x <= sushi->x + sushi->xpm->width &&
        cut_y >= sushi->y && cut_y <= sushi->y + sushi->xpm->height) {
          return cut_sushi(sushi, left_fragment, right_fragment);
    }

    return 1;
}

int cut_sushi(sushi_t* sushi, sushi_fragment_t* left_fragment, sushi_fragment_t* right_fragment) {
    if (sushi == NULL || left_fragment == NULL || right_fragment == NULL || sushi->type >= 11) return 1;

    sprite_t *left_sprite = malloc(sizeof(sprite_t));
    sprite_t *right_sprite = malloc(sizeof(sprite_t));

    if (left_sprite == NULL || right_sprite == NULL) {
        free(left_sprite);
        free(right_sprite);
        return 1;
    }

    if (sprite_load(left_sprite, sushi_left_xpms[sushi->type])) {
        free(left_sprite);
        free(right_sprite);
        return 1;
    }

    if (sprite_load(right_sprite, sushi_right_xpms[sushi->type])) {
        sprite_destroy(left_sprite);
        free(left_sprite);
        free(right_sprite);
        return 1;
    }

    // Create left fragment
    *left_fragment = (sushi_fragment_t){
        .x = sushi->x,
        .y = sushi->y,
        .vx = sushi->vx - 2, // Move left
        .vy = sushi->vy - 2, // Move down
        .xpm = left_sprite
    };

    // Create right fragment
    *right_fragment = (sushi_fragment_t){
        .x = sushi->x,
        .y = sushi->y,
        .vx = sushi->vx + 2, // Move right
        .vy = sushi->vy - 2, // Move down
        .xpm = right_sprite
    };

    // Destroy the original sushi
    sushi_destroy(sushi);

    return 0;
}

void update_sushi_fragment(sushi_fragment_t* fragment) {
    if (fragment == NULL) return;

    fragment->x += fragment->vx;
    fragment->y += fragment->vy;

    fragment->vy -= 1; // Simulate gravity

    // Deactivate if it goes off-screen
    if (fragment->x < 0 || fragment->x > SCREEN_WIDTH ||
        fragment->y < 0 || fragment->y > SCREEN_HEIGHT) {
          sushi_fragment_destroy(fragment);
    }
}

void sushi_fragment_destroy(sushi_fragment_t* fragment) {
    if (fragment != NULL) {
        if (fragment->xpm != NULL) {
            sprite_destroy(fragment->xpm);
            free(fragment->xpm);
            fragment->xpm = NULL;
        }
    }
}
