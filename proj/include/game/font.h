#ifndef _FONT_H_
#define _FONT_H_

#include "sprite.h"

typedef struct {
  sprite_t **sprites;
  uint16_t char_width;
  uint32_t num_chars;
} font_t;

int font_create(font_t *font, uint16_t char_width);
void font_destroy(font_t *font);

int font_glyph_index(char c);

#endif /* _FONT_H_ */
