#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

#include <stdint.h>
#include "vesa-bios.h"

int (set_video_mode)(uint16_t mode);
void* (vg_init)(uint16_t mode);
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
int (vg_draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y);
int (vg_exit)(void);
int (vg_display_vbe_contr_info)(struct vg_vbe_contr_info *info_p);

#endif /* __VIDEO_GR_H */
