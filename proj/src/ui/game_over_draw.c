#include "../../include/ui/game_over_draw.h"

#include <stdio.h>

#include "../../include/macros/menu.h"
#include "../../include/ui/ui.h"

typedef struct {
    GameOverOption option;
    button_t button;
} GameOverButtonSpec;

static button_t make_game_over_button(uint16_t x, uint16_t y, const char *label) {
    button_t button;
    button.x = x;
    button.y = y;
    button.width = GAME_OVER_BUTTON_W;
    button.height = GAME_OVER_BUTTON_H;
    button.color = MENU_BUTTON_COLOR;
    button.hover_color = MENU_BUTTON_HOVER_COLOR;
    button.label = label;
    button.hovered = false;
    return button;
}

static uint16_t button_text_x(const button_t *button, const Font *font) {
    uint16_t width = font_text_width(font, button->label);

    if (width >= button->width) return button->x;
    return button->x + (button->width - width) / 2;
}

static uint16_t button_text_y(const button_t *button, const Font *font) {
    if (font->char_width >= button->height) return button->y;
    return button->y + (button->height - font->char_width) / 2;
}

static int draw_button_with_label(const button_t *button, const Font *font) {
    if (ui_draw_button(button) != 0) return 1;
    return ui_draw_text(font,
                        button->label,
                        button_text_x(button, font),
                        button_text_y(button, font));
}

static GameOverButtonSpec game_over_button(void) {
    GameOverButtonSpec spec;
    spec.option = GAME_OVER_OPTION_MAIN_MENU;
    spec.button = make_game_over_button(GAME_OVER_BUTTON_X,
                                        GAME_OVER_BUTTON_Y,
                                        "main menu");
    return spec;
}

GameOverOption ui_game_over_hit_test(int x, int y) {
    GameOverButtonSpec spec = game_over_button();
    if (ui_button_contains(&spec.button, x, y)) {
        return spec.option;
    }
    return GAME_OVER_OPTION_NONE;
}

int ui_game_over_draw(const game_over_t *game_over, const Assets *assets) {
    if (game_over == NULL || assets == NULL) return 1;

    char score_text[24];
    snprintf(score_text, sizeof(score_text), "score %u", game_over->final_score);

    if (ui_draw_background(&assets->background_sprite) != 0) return 1;

    if (font_draw_scaled_text(&assets->game_font,
                              "GAME OVER",
                              CENTER_X(font_scaled_text_width(&assets->game_font, "GAME OVER", GAME_OVER_TITLE_SCALE)),
                              GAME_OVER_TITLE_Y,
                              GAME_OVER_TITLE_SCALE) != 0) {
        return 1;
    }

    if (font_draw_scaled_text(&assets->game_font,
                              score_text,
                              CENTER_X(font_scaled_text_width(&assets->game_font, score_text, GAME_OVER_SCORE_SCALE)),
                              GAME_OVER_SCORE_Y,
                              GAME_OVER_SCORE_SCALE) != 0) {
        return 1;
    }

    anim_draw(&assets->mascot_lose_anim, GAME_OVER_MASCOT_X, GAME_OVER_MASCOT_Y);

    GameOverButtonSpec spec = game_over_button();
    spec.button.hovered = spec.option == game_over->hovered_option;

    return draw_button_with_label(&spec.button, &assets->game_font);
}
