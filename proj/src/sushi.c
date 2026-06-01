
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
    // Placeholder XPMs for different sushi types
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
    // Placeholder XPMs for different sushi types
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
    return sushi;
}

sushi_t create_random_sushi() {
    sprite_t sushi_xpm_sprite;
    if (sprite_load(&sushi_xpm_sprite, sushi_xpms[rand() % 11])) {
        // Return a default sushi in case of failure
        return create_sushi(0, 0, 0, 0, NULL, 0);
    }
    return create_sushi(rand() % (SCREEN_WIDTH - 100) + 50, 0, rand() % 10 - 5, rand() % 10, &sushi_xpm_sprite, (rand() % 5 + 1) * 10);
}

void update_sushi(sushi_t* sushi) {
    if (sushi == NULL) return;

    sushi->x += sushi->vx;
    sushi->y += sushi->vy;

    sushi->vy -= 1; // Simulate gravity

    // Deactivate if it goes off-screen
    if (sushi->x < 0 || sushi->x > SCREEN_WIDTH ||
        sushi->y < 0 || sushi->y > SCREEN_HEIGHT) {
          sushi_destroy(&sushi);
    }
}

void sushi_destroy(sushi_t* sushi) {
    if (sushi != NULL) {
        if (sushi->xpm != NULL) {
            sprite_destroy(&sushi->xpm);
            sushi = NULL;
        }
    }
}

void find_cut_sushi(sushi_t* sushi, int cut_x, int cut_y) {
    if (sushi == NULL) return;

    // Check if the cut coordinates are within the sushi's bounding box
    if (cut_x >= sushi->x && cut_x <= sushi->x + sushi->xpm->width &&
        cut_y >= sushi->y && cut_y <= sushi->y + sushi->xpm->height) {
          for (int i=0; i<11; i++) {
              if (sushi->xpm->pixmap == (uint8_t*)sushi_xpms[i]) {
                  // Found the sushi type, now cut it

                  sushi_fragment_t sushi_left_fragment, sushi_right_fragment;
                  cut_sushi(&sushi, &sushi_left_xpms[i], &sushi_right_xpms[i]);
                  break;
              }
          }
    }
}

void cut_sushi(sushi_t* sushi, sushi_fragment_t* left_fragment, sushi_fragment_t* right_fragment) {
    if (sushi == NULL || left_fragment == NULL || right_fragment == NULL) return;

    // Create left fragment
    *left_fragment = (sushi_fragment_t){
        .x = sushi->x,
        .y = sushi->y,
        .vx = sushi->vx - 2, // Move left
        .vy = sushi->vy - 2, // Move down
        .xpm = create_sprite(sushi_left_xpms[rand() % 11]) // Random left fragment XPM
    };

    // Create right fragment
    *right_fragment = (sushi_fragment_t){
        .x = sushi->x,
        .y = sushi->y,
        .vx = sushi->vx + 2, // Move right
        .vy = sushi->vy - 2, // Move down
        .xpm = create_sprite(sushi_right_xpms[rand() % 11]) // Random right fragment XPM
    };

    // Destroy the original sushi
    sushi_destroy(sushi);
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
            sprite_destroy(&fragment->xpm);
            fragment = NULL;
        }
    }
}