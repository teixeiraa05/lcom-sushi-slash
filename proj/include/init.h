#ifndef __INIT_H
#define __INIT_H

#include "game/sprite.h"
#include "font.h"


typedef struct {
  sprite_t background_sprite;
  sprite_t name_sprite;
  Font game_font;
  sprite_t mouse_cursor;
  sprite_t heart;

  sprite_t *mascote[4]; //array of pointers to the animation frames
  anim_sprite_t mascot_anim; //animation struct for the mascot
} Assets;

int assets_init(Assets *assets);
void assets_destroy(Assets *assets);

#endif /* __INIT_H */
