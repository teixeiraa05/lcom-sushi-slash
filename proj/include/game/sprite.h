#ifndef __SPRITE_H
#define __SPRITE_H

#include <lcom/lcf.h>

typedef struct {
    char* pixmap;
    uint16_t width, height;
} sprite_t;

typedef struct {
    sprite_t** sprites; // An array of pointers to sprites, each pointer is a frame of the animation
    size_t num_sprites;
} animated_sprite_t;

int sprite_load(sprite_t *sprite, xpm_map_t xpm);
void sprite_destroy(sprite_t *sprite);

#endif /* __SPRITE_H */
