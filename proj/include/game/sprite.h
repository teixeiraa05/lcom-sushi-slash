#ifndef __SPRITE_H
#define __SPRITE_H

#define ANIM_SLOW_DELAY 20
#define ANIM_NORMAL_DELAY 10
#define ANIM_FAST_DELAY 5

typedef struct {
    uint8_t* pixmap;
    uint16_t width, height;
} sprite_t;

typedef struct {
    sprite_t** sprites; // Array of pointers to sprites, each pointer is a frame
    size_t num_sprites;
    uint8_t cur_frame;
    uint32_t last_tick;
    int   frame_delay;  // ticks between frames
} anim_sprite_t;

int sprite_load(sprite_t *sprite, xpm_map_t xpm);
void sprite_destroy(sprite_t *sprite);

int anim_sprite_load(anim_sprite_t *anim, sprite_t *sprites[], size_t count,
                     xpm_map_t xpms[], int frame_delay);

#endif /* __SPRITE_H */
