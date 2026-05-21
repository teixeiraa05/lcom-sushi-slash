#ifndef __VIDEO_PROJ_H
#define __VIDEO_PROJ_H

#include <lcom/lcf.h>
#include <stdint.h>

/**
 * @brief Initializes double buffering by allocating the secondary back buffer in RAM.
 * 
 * @param mode The 16-bit VESA graphics mode to initialize.
 * @return 0 on success, non-zero on failure.
 */
int video_init_double_buffering(uint16_t mode);

/**
 * @brief Clears the entire back buffer with a uniform color.
 * 
 * @return 0 on success, non-zero on failure.
 */
int video_clear_buffer(void);

/**
 * @brief Swaps the buffers by copying the entire back buffer content to VRAM.
 * 
 * Must be called exactly once per tick at the end of the rendering phase.
 * 
 * @return 0 on success, non-zero on failure.
 */
int video_swap_buffers(void);

/**
 * @brief Cleans up double buffering resources by freeing the allocated back buffer.
 * 
 * @return 0 on success, non-zero on failure.
 */
int video_cleanup_double_buffering(void);

#endif /* __VIDEO_PROJ_H */
