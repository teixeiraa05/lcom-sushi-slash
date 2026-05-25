#include <lcom/lcf.h>
#include "../include/game/sprite.h"

int sprite_load(sprite_t *sprite, xpm_map_t xpm) {
    if (sprite == NULL) return 1;

    xpm_image_t img;
    uint8_t *pixmap = xpm_load(xpm, XPM_8_8_8, &img);
    if (pixmap == NULL) {
        printf("[sprite] failed to load xpm\n");
        return 1;
    }

    sprite->pixmap = pixmap;
    sprite->width = img.width;
    sprite->height = img.height;

    return 0;
}

void sprite_destroy(sprite_t *sprite) {
    if (sprite != NULL && sprite->pixmap != NULL) {
        free(sprite->pixmap);
        sprite->pixmap = NULL;
    }
}
