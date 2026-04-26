#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

#include <stdint.h>
#include "vesa-bios.h"

void* (vg_init)(uint16_t mode);
int (vg_graw_hline) (uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int (vg_draw_rectangle) (uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);


#endif /* __VIDEO_GR_H */
