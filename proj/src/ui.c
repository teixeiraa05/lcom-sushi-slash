#include <lcom/lcf.h>
#include <stdint.h>
#include <string.h>

#include "../include/ui.h"
#include "../include/devices/video_gr.h"
#include "../include/game/button.h"
#include "../include/game/menu.h"

int ui_draw_sprite(const sprite_t *sprite, uint16_t x, uint16_t y) {
    if (sprite == NULL || sprite->pixmap == NULL) {
        printf("[ui] sprite or pixmap is NULL\n");
        return 1;
    }
    return video_draw_sprite((const uint8_t *) sprite->pixmap, sprite->width, sprite->height, x, y);
}

int ui_draw_background(const sprite_t *background) {
    if (background == NULL || background->pixmap == NULL) {
        printf("[ui] background sprite or pixmap is NULL\n");
        return 1;
    }
    return video_draw_background((const uint8_t *) background->pixmap);
}

int ui_draw_rectangle(int x, int y, int width, int height, uint32_t color) {
    if (width <= 0 || height <= 0) return 0;
    
    // Draw filled solid rectangle by plotting pixels
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            if (video_draw_pixel(x + col, y + row, color) != 0) {
                return 1;
            }
        }
    }
    return 0;
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
