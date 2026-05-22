#ifndef __VIDEO_PROJ_H
#define __VIDEO_PROJ_H

#include <lcom/lcf.h>
#include <stdint.h>

#define VBE_SET_DISPLAY_START 0x07

/**
 * @brief Initializes page-flipping by mapping double the VRAM size (2 pages).
 * 
 * @param mode The 16-bit VESA graphics mode to initialize.
 * @return 0 on success, non-zero on failure.
 */
int video_init_page_flipping(uint16_t mode);

/**
 * @brief Clears the inactive rendering page with black (0).
 * 
 * @return 0 on success, non-zero on failure.
 */
int video_clear_page(void);

/**
 * @brief Swaps the visible and rendering pages via hardware page-flipping (VBE 0x07).
 * 
 * Must be called exactly once per tick at the end of the rendering phase.
 * 
 * @return 0 on success, non-zero on failure.
 */
int video_swap_pages(void);

/**
 * @brief Cleans up page-flipping resources.
 * 
 * @return 0 on success, non-zero on failure.
 */
int video_cleanup_page_flipping(void);

/**
 * @brief Renders the entire background onto the inactive (back) page in VRAM.
 * 
 * High-performance operation that uses direct memory copying.
 * 
 * @param background_pixmap Pointer to the raw background pixmap data (RGB888).
 * @return 0 on success, non-zero on failure.
 */
int video_draw_background(const uint8_t *background_pixmap);

/**
 * @brief Draws a single pixel directly onto the inactive (back) page in VRAM.
 * 
 * @param x Horizontal pixel position from left (0 to width - 1).
 * @param y Vertical pixel position from top (0 to height - 1).
 * @param color The raw pixel color value.
 * @return 0 on success, non-zero on failure.
 */
int video_draw_pixel(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Draws a sprite's pixmap directly onto the inactive (back) page in VRAM.
 * 
 * High-performance operation that uses direct pointer arithmetic and ignores chroma-key green transparency.
 * 
 * @param pixmap Pointer to the raw sprite pixmap data (RGB888).
 * @param width Sprite width in pixels.
 * @param height Sprite height in pixels.
 * @param x Horizontal pixel position from left.
 * @param y Vertical pixel position from top.
 * @return 0 on success, non-zero on failure.
 */
int video_draw_sprite(const uint8_t *pixmap, uint16_t width, uint16_t height, uint16_t x, uint16_t y);

#endif /* __VIDEO_PROJ_H */
