
#include "../../include/ui/ui.h"
#include "../../include/ui/menu_draw.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define CENTER_X(width) ((SCREEN_WIDTH - (width)) / 2)
#define CENTER_Y(height) ((SCREEN_HEIGHT - (height)) / 2)

#define MENU_BUTTON_W 300
#define MENU_BUTTON_H 56
#define MENU_BUTTON_GAP 14
#define MENU_BUTTON_X CENTER_X(MENU_BUTTON_W)
#define MENU_BUTTON_Y (SCREEN_HEIGHT / 2)
#define MENU_BUTTON_COLOR 0x303030
#define MENU_BUTTON_HOVER_COLOR 0x606060
#define MENU_MASCOT_X 560
#define MENU_MASCOT_Y 180

#define PAUSE_PANEL_W 400
#define PAUSE_PANEL_H 300
#define PAUSE_PANEL_X CENTER_X(PAUSE_PANEL_W)
#define PAUSE_PANEL_Y CENTER_Y(PAUSE_PANEL_H)
#define PAUSE_BUTTON_W 220
#define PAUSE_BUTTON_H 42
#define PAUSE_BUTTON_GAP 12
#define PAUSE_BUTTON_COUNT 2
#define PAUSE_BUTTONS_H (PAUSE_BUTTON_COUNT * PAUSE_BUTTON_H + (PAUSE_BUTTON_COUNT - 1) * PAUSE_BUTTON_GAP)
#define PAUSE_BUTTON_X CENTER_X(PAUSE_BUTTON_W)
#define PAUSE_BUTTON_Y (PAUSE_PANEL_Y + (PAUSE_PANEL_H - PAUSE_BUTTONS_H) / 2)
#define PAUSE_MASCOT_X 500
#define PAUSE_MASCOT_Y 250

typedef struct {
    MenuOption option;
    button_t button;
} MenuButtonSpec;

typedef struct {
    PauseOption option;
    button_t button;
} PauseButtonSpec;

static button_t make_button(uint16_t x, uint16_t y, const char *label) {
    button_t button;
    button.x = x;
    button.y = y;
    button.width = MENU_BUTTON_W;
    button.height = MENU_BUTTON_H;
    button.color = MENU_BUTTON_COLOR;
    button.hover_color = MENU_BUTTON_HOVER_COLOR;
    button.label = label;
    button.hovered = false;
    return button;
}

static uint16_t text_width(const Font *font, const char *text) {
    uint16_t width = 0;

    if (font == NULL || text == NULL) return 0;

    for (const char *p = text; *p != '\0'; p++) {
        width += font->char_width;
    }

    return width;
}

static uint16_t button_text_x(const button_t *button, const Font *font) {
    uint16_t width = text_width(font, button->label);

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

static MenuButtonSpec menu_button_at(uint8_t index) {
    static const MenuOption options[] = {
        MENU_OPTION_SINGLEPLAYER,
        MENU_OPTION_MULTIPLAYER,
        MENU_OPTION_INSTRUCTIONS,
        MENU_OPTION_EXIT
    };
    static const char *labels[] = {
        "singleplayer",
        "multiplayer",
        "instructions",
        "exit"
    };

    MenuButtonSpec spec;
    spec.option = options[index];
    spec.button = make_button(MENU_BUTTON_X,
                              MENU_BUTTON_Y + index * (MENU_BUTTON_H + MENU_BUTTON_GAP),
                              labels[index]);
    return spec;
}

static PauseButtonSpec pause_button_at(uint8_t index) {
    static const PauseOption options[] = {
        PAUSE_OPTION_RESUME,
        PAUSE_OPTION_EXIT
    };
    static const char *labels[] = {
        "resume",
        "exit"
    };

    PauseButtonSpec spec;
    spec.option = options[index];
    spec.button = make_button(PAUSE_BUTTON_X,
                              PAUSE_BUTTON_Y + index * (PAUSE_BUTTON_H + PAUSE_BUTTON_GAP),
                              labels[index]);
    spec.button.width = PAUSE_BUTTON_W;
    spec.button.height = PAUSE_BUTTON_H;
    return spec;
}

MenuOption ui_menu_hit_test(int x, int y) {
    for (uint8_t i = 0; i < 4; i++) {
        MenuButtonSpec spec = menu_button_at(i);
        if (ui_button_contains(&spec.button, x, y)) {
            return spec.option;
        }
    }
    return MENU_OPTION_NONE;
}

PauseOption ui_pause_menu_hit_test(int x, int y) {
    for (uint8_t i = 0; i < 2; i++) {
        PauseButtonSpec spec = pause_button_at(i);
        if (ui_button_contains(&spec.button, x, y)) {
            return spec.option;
        }
    }
    return PAUSE_OPTION_NONE;
}

int ui_menu_draw(const menu_t *menu, const Assets *assets) {
    if (menu == NULL || assets == NULL) return 1;

    if (ui_draw_background(&assets->background_sprite) != 0) return 1;
    if (ui_draw_sprite(&assets->name_sprite, 50, 100) != 0) return 1;
    anim_draw(&assets->mascot_anim, MENU_MASCOT_X, MENU_MASCOT_Y);

    for (uint8_t i = 0; i < 4; i++) {
        MenuButtonSpec spec = menu_button_at(i);
        spec.button.hovered = spec.option == menu->hovered_option;

        if (draw_button_with_label(&spec.button, &assets->game_font) != 0) {
            return 1;
        }
    }

    return 0;
}

int ui_pause_menu_draw(const pause_menu_t *menu, const Assets *assets) {
    if (menu == NULL || assets == NULL) return 1;

    if (ui_draw_rectangle(PAUSE_PANEL_X, PAUSE_PANEL_Y,
                          PAUSE_PANEL_W, PAUSE_PANEL_H,
                          0x1A1A1A) != 0) {
        return 1;
    }
    anim_draw(&assets->mascot_anim, PAUSE_MASCOT_X, PAUSE_MASCOT_Y);

    for (uint8_t i = 0; i < 2; i++) {
        PauseButtonSpec spec = pause_button_at(i);
        spec.button.hovered = spec.option == menu->hovered_option;

        if (draw_button_with_label(&spec.button, &assets->game_font) != 0) {
            return 1;
        }
    }

    return 0;
}
