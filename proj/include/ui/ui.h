#ifndef __UI_PROJ_H
#define __UI_PROJ_H

#include <stdbool.h>
#include <stdint.h>
#include "../game/sprite.h"
#include "../font.h"
#include "button.h"

int ui_draw_pixel(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Renders a standard sprite at coordinate (x, y) on the VRAM back page.
 * 
 * Automatically handles bounds clipping and ignores green chroma key background pixels.
 * 
 * @param sprite Pointer to the sprite structure to render.
 * @param x Horizontal pixel position from left.
 * @param y Vertical pixel position from top.
 * @return 0 on success, non-zero on failure.
 */
int ui_draw_sprite(const sprite_t *sprite, uint16_t x, uint16_t y);

/**
 * @brief Renders the entire full-screen background sprite.
 * 
 * High-performance operation that uses direct memory copying.
 * 
 * @param background Pointer to the background sprite.
 * @return 0 on success, non-zero on failure.
 */
int ui_draw_background(const sprite_t *background);

/**
 * @brief Draws a filled solid rectangle onto the active drawing page.
 * 
 * @param x Top-left horizontal coordinate.
 * @param y Top-left vertical coordinate.
 * @param width Width of the rectangle in pixels.
 * @param height Height of the rectangle in pixels.
 * @param color Color value to fill.
 * @return 0 on success, non-zero on failure.
 */
int ui_draw_rectangle(int x, int y, int width, int height, uint32_t color);

/**
 * @brief Renders a button onto the screen, applying hover state colors automatically.
 * 
 * @param btn Pointer to the button structure to draw.
 * @return 0 on success, non-zero on failure.
 */
int ui_draw_button(const button_t *btn);
bool ui_button_contains(const button_t *btn, int x, int y);

void anim_update(anim_sprite_t *anim, int current_tick);
void anim_draw(const anim_sprite_t *anim, uint16_t x, uint16_t y);

int ui_draw_text(const Font *font, const char *str, uint16_t x, uint16_t y);

void hud_draw(const Font *font, uint32_t score, uint8_t lives, const sprite_t * heart);

#endif /* __UI_PROJ_H */
