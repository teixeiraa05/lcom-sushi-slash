#include <lcom/lcf.h>
#include <stdint.h>
#include <string.h>

#include "../include/ui.h"
#include "../../lab5/video_gr.h"
#include "../include/game/button.h"
#include "../include/game/menu.h"
#include "../include/game/font.h"

int ui_draw_sprite(const sprite_t *sprite, uint16_t x, uint16_t y) {
    if (sprite == NULL || sprite->pixmap == NULL) {
        printf("[ui] sprite or pixmap is NULL\n");
        return 1;
    }
    return video_draw_sprite(sprite->pixmap, sprite->width, sprite->height, x, y);
}

void anim_update(anim_sprite_t *anim, int current_tick) {
    if (current_tick - anim->last_tick >= anim->frame_delay) {
        anim->cur_frame = (anim->cur_frame + 1) % anim->num_sprites;
        anim->last_tick = current_tick;
    }
}

void anim_draw(const anim_sprite_t *anim, uint16_t x, uint16_t y) {
    ui_draw_sprite(anim->sprites[anim->cur_frame], x, y);
}

int ui_draw_background(const sprite_t *background) {
    if (background == NULL || background->pixmap == NULL) {
        printf("[ui] background sprite or pixmap is NULL\n");
        return 1;
    }
    return video_draw_background(background->pixmap);
}

int ui_draw_rectangle(int x, int y, int width, int height, uint32_t color) {
    /* clamp negative origin: shrink dimensions accordingly */
    if (x < 0) { width  += x; x = 0; }
    if (y < 0) { height += y; y = 0; }
    if (width <= 0 || height <= 0) return 0;

    return video_draw_rect((uint16_t)x, (uint16_t)y,
                           (uint16_t)width, (uint16_t)height, color);
}

int ui_draw_button(const button_t *btn) {
    if (btn == NULL) {
        return 1;
    }
    // Determine current color based on mouse hover state
    uint32_t draw_color = btn->hovered ? btn->hover_color : btn->color;
    return ui_draw_rectangle(btn->x, btn->y, btn->width, btn->height, draw_color);
}

int ui_draw_main_menu(const GameInfo *game) {
    if (game == NULL) {
        return 1;
    }
    //TODO
    
    return 0;
}

int ui_draw_pause_screen(const GameInfo *game) {
    if (game == NULL) {
        return 1;
    }

    // Renders the pause overlay template
    // Draw a semi-transparent dark grey rectangle in the center of the screen
    // (e.g. at x = 200, y = 150, width = 400, height = 300) to dim the gameplay
    uint32_t overlay_color = 0x1A1A1A; // Dark grey
    if (ui_draw_rectangle(200, 150, 400, 300, overlay_color) != 0) {
        return 1;
    }

    return 0;
}

int ui_draw_game_over(const GameInfo *game) {
    if (game == NULL) {
        return 1;
    }

   // TODO 

    return 0;
}

int ui_draw_text(const font_t *font, const char *str, uint16_t x, uint16_t y) {
    if (font == NULL || str == NULL)
        return 1;

    uint16_t cur_x = x;


    for (const char *p = str; *p != '\0'; p++) {
        if (*p == ' ') {
            cur_x += font->char_width;
            continue;
        }

        int idx = font_glyph_index(*p);
        if (idx < 0) {
            cur_x += font->char_width;
            continue;
        }

        if (ui_draw_sprite(font->sprites[idx], cur_x, y) != 0)
            return 1;
        cur_x += font->char_width;
    }
    return 0;
}
