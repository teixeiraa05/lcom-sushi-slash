#ifndef __INIT_H
#define __INIT_H

#include "sprite.h"
#include "font.h"
#include "../ui.h"


typedef struct {
  sprite_t background_sprite;
  sprite_t test_sprite;
  font_t game_font;

  sprite_t *mascote[4]; //array of pointers to the animation frames
  anim_sprite_t mascot_anim; //animation struct for the mascot
} Assets;

int assets_init(Assets *assets);
void assets_destroy(Assets *assets);

#endif /* __INIT_H */
