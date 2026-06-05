#include "../include/game/slash_combo.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../include/macros/global.h"
#include "../include/ui/ui.h"

static void slash_combo_reset_active(slash_combo_t *combo) {
    if (combo == NULL) return;

    combo->count = 0;
    combo->last_cut_tick = 0;
    combo->base_points = 0;
    combo->impact_x = 0;
    combo->impact_y = 0;
}

static uint32_t slash_combo_finish(slash_combo_t *combo) {
    if (combo == NULL) return 0;

    uint32_t bonus = 0;

    if (combo->count >= SLASH_COMBO_MIN_CUTS) {
        bonus = combo->base_points;
        combo->popup_ticks = SLASH_COMBO_POPUP_TICKS;
        combo->popup_count = combo->count;
        combo->popup_x = combo->impact_x;
        combo->popup_y = combo->impact_y;
    }

    slash_combo_reset_active(combo);
    return bonus;
}

static bool slash_combo_expired(const slash_combo_t *combo, uint32_t current_tick) {
    return combo != NULL &&
           combo->count > 0 &&
           current_tick - combo->last_cut_tick > SLASH_COMBO_WINDOW_TICKS;
}

static int slash_combo_clamp_position(int value, int size, int screen_size) {
    int max_value = screen_size - size;

    if (max_value < 0) max_value = 0;
    if (value < 0) return 0;
    if (value > max_value) return max_value;
    return value;
}

static void slash_combo_draw_plus(int x, int y, uint8_t scale, uint32_t color) {
    int size = 8 * scale;
    int thickness = 2 * scale;
    int half = size / 2;

    ui_draw_rectangle(x, y + half - thickness / 2, size, thickness, color);
    ui_draw_rectangle(x + half - thickness / 2, y, thickness, size, color);
}

void slash_combo_reset(slash_combo_t *combo) {
    if (combo == NULL) return;

    memset(combo, 0, sizeof(*combo));
}

uint32_t slash_combo_register_cut(slash_combo_t *combo,
                                  uint32_t current_tick,
                                  uint32_t score_value,
                                  int impact_x,
                                  int impact_y) {
    if (combo == NULL) return 0;

    uint32_t bonus = 0;
    if (slash_combo_expired(combo, current_tick)) {
        bonus = slash_combo_finish(combo);
    }

    if (combo->count < UINT8_MAX) {
        combo->count++;
    }

    combo->last_cut_tick = current_tick;
    combo->base_points += score_value;
    combo->impact_x = impact_x;
    combo->impact_y = impact_y;

    return bonus;
}

uint32_t slash_combo_update(slash_combo_t *combo, uint32_t current_tick) {
    if (combo == NULL) return 0;

    if (slash_combo_expired(combo, current_tick)) {
        return slash_combo_finish(combo);
    }

    if (combo->popup_ticks > 0) {
        combo->popup_ticks--;
    }

    return 0;
}

void slash_combo_draw(const slash_combo_t *combo, const Assets *assets) {
    if (combo == NULL || assets == NULL ||
        combo->popup_ticks == 0 ||
        combo->popup_count < SLASH_COMBO_MIN_CUTS) {
        return;
    }

    const Font *font = &assets->game_font;
    char bonus_text[4];
    snprintf(bonus_text, sizeof(bonus_text), "%u", (unsigned)combo->popup_count);

    uint8_t scale = 2;
    uint16_t title_width = font_scaled_text_width(font, "COMBO", scale);
    int title_x = slash_combo_clamp_position(combo->popup_x - title_width / 2,
                                             title_width,
                                             SCREEN_WIDTH);
    int title_y = slash_combo_clamp_position(combo->popup_y - 72,
                                             56,
                                             SCREEN_HEIGHT);

    font_draw_scaled_text_color(font,
                                "COMBO",
                                (uint16_t)title_x,
                                (uint16_t)title_y,
                                scale,
                                SLASH_COMBO_POPUP_COLOR);

    int plus_size = 8 * scale;
    int gap = 4 * scale;
    uint16_t number_width = font_scaled_text_width(font, bonus_text, scale);
    int line_width = plus_size + gap + number_width;
    int line_x = slash_combo_clamp_position(combo->popup_x - line_width / 2,
                                            line_width,
                                            SCREEN_WIDTH);
    int line_y = slash_combo_clamp_position(title_y + 32,
                                            24,
                                            SCREEN_HEIGHT);

    slash_combo_draw_plus(line_x, line_y + 4, scale, SLASH_COMBO_POPUP_COLOR);
    font_draw_scaled_text_color(font,
                                bonus_text,
                                (uint16_t)(line_x + plus_size + gap),
                                (uint16_t)line_y,
                                scale,
                                SLASH_COMBO_POPUP_COLOR);
}
