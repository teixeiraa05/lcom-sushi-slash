#ifndef _FONT_H_
#define _FONT_H_

#include <stdint.h>
#include "game/sprite.h"

#define FONT_NUM_DIGITS 10
#define FONT_NUM_LETTERS 26
#define FONT_NUM_SYMBOLS 2
#define FONT_NUM_GLYPHS (FONT_NUM_DIGITS + FONT_NUM_LETTERS + FONT_NUM_SYMBOLS)
#define FONT_COLON_INDEX (FONT_NUM_DIGITS + FONT_NUM_LETTERS)
#define FONT_SLASH_INDEX (FONT_COLON_INDEX + 1)

typedef struct {
  sprite_t glyphs[FONT_NUM_GLYPHS];
  uint16_t char_width;
  uint32_t num_chars;
  uint32_t loaded_chars;
} Font;

int font_create(Font *font, uint16_t char_width);
void font_destroy(Font *font);

int font_glyph_index(char c);
uint16_t font_text_width(const Font *font, const char *text);
uint16_t font_scaled_text_width(const Font *font, const char *text, uint8_t scale);
uint16_t font_half_text_width(const Font *font, const char *text);
int font_draw_scaled_text(const Font *font, const char *text, uint16_t x, uint16_t y, uint8_t scale);
int font_draw_scaled_text_color(const Font *font, const char *text, uint16_t x, uint16_t y, uint8_t scale, uint32_t color);
int font_draw_half_text_color(const Font *font, const char *text, uint16_t x, uint16_t y, uint32_t color);

#endif /* _FONT_H_ */
