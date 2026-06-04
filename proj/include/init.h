#ifndef __INIT_H
#define __INIT_H

#include "game/sprite.h"
#include "font.h"

#define MASCOT_IDLE_FRAME_COUNT 4
#define MASCOT_LOSE_FRAME_COUNT 15

typedef struct {
  sprite_t background_sprite;
  sprite_t name_sprite;
  Font game_font;
  sprite_t mouse_cursor;
  sprite_t heart;
  sprite_t bomb;

  sprite_t *mascote[MASCOT_IDLE_FRAME_COUNT]; //array of pointers to the animation frames
  anim_sprite_t mascot_anim; //animation struct for the mascot
  sprite_t *mascote_lose[MASCOT_LOSE_FRAME_COUNT];
  anim_sprite_t mascot_lose_anim;
} Assets;

int assets_init(Assets *assets);
void assets_destroy(Assets *assets);

#endif /* __INIT_H */
