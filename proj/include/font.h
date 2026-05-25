#ifndef _FONT_H_
#define _FONT_H_

#include <stdint.h>
#include "game/sprite.h"

#define FONT_NUM_DIGITS 10
#define FONT_NUM_LETTERS 26
#define FONT_NUM_GLYPHS (FONT_NUM_DIGITS + FONT_NUM_LETTERS)

typedef struct {
  sprite_t glyphs[FONT_NUM_GLYPHS];
  uint16_t char_width;
  uint32_t num_chars;
  uint32_t loaded_chars;
} Font;

int font_create(Font *font, uint16_t char_width);
void font_destroy(Font *font);

int font_glyph_index(char c);
#endif /* _FONT_H_ */
