#include <lcom/lcf.h>
#include <stdint.h>
#include <string.h>

#include "../include/devices/video_gr.h"
#include "../../lab5/video_gr.h"
#include "../../lab5/vesa-bios.h"

// File-local static states for VRAM double buffering (page-flipping)
static uint8_t *vram_base_ptr = NULL;  // Base address of the mapped VRAM
static unsigned int vram_size = 0;     // Total mapped VRAM size (2 frames)
static unsigned int page_size = 0;     // Size of a single screen frame in bytes
static uint16_t h_res = 0;             // Screen width
static uint16_t v_res = 0;             // Screen height
static uint8_t bytes_per_pixel = 0;    // Bytes per pixel
static uint8_t active_draw_page = 1;   // The page currently targeted for drawing (0 or 1)

/**
 * @brief Helper function that calls VBE Function 0x07 (Set Display Start) via real-mode BIOS interrupt.
 * 
 * Tells the graphics controller which horizontal and vertical offset to use as the top-left of the display.
 * 
 * @param x Horizontal start offset in pixels.
 * @param y Vertical start offset in lines.
 * @return 0 on success, non-zero on failure.
 */
static int video_set_display_start(uint16_t x, uint16_t y) {
    reg86_t reg;
    memset(&reg, 0, sizeof(reg));
    
    reg.intno = VBE_INT;
    reg.ah = AH_VBE;
    reg.al = VBE_SET_DISPLAY_START;
    reg.bx = 0x00;       // Set Display Start
    reg.cx = x;          // X offset in pixels
    reg.dx = y;          // Y offset in lines

    if (sys_int86(&reg) != OK) {
        printf("[video_gr] sys_int86 (VBE Set Display Start) failed\n");
        return 1;
    }
    return 0;
}

int video_init_page_flipping(uint16_t mode) {
    vbe_mode_info_t vmi;
    memset(&vmi, 0, sizeof(vmi));
    if (vbe_get_mode_info(mode, &vmi) != OK) {
        printf("[video_gr] vbe_get_mode_info failed\n");
        return 1;
    }

    h_res = vmi.XResolution;
    v_res = vmi.YResolution;
    bytes_per_pixel = (vmi.BitsPerPixel + 7) / 8;
    
    page_size = h_res * v_res * bytes_per_pixel;
    vram_size = page_size * 2; // Map exactly 2 full pages of VRAM to support page flipping

    int r;
    struct minix_mem_range mr;
    mr.mr_base = (phys_bytes) vmi.PhysBasePtr;
    mr.mr_limit = mr.mr_base + vram_size;

    if ((r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)) != OK) {
        printf("[video_gr] sys_privctl (ADD_MEM) failed: %d\n", r);
        return 1;
    }

    //Map physical VRAM to virtual process space
    vram_base_ptr = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
    if (vram_base_ptr == MAP_FAILED) {
        printf("[video_gr] vm_map_phys failed to map VRAM memory\n");
        return 1;
    }

    //Set the VESA video mode using the mode setup logic in lab5
    if (set_video_mode(mode) != 0) {
        printf("[video_gr] set_video_mode failed\n");
        return 1;
    }

    //Initialize page rendering: page 0 is displayed, page 1 is standard drawing target
    active_draw_page = 1;
    if (video_set_display_start(0, 0) != 0) {
        printf("[video_gr] failed to configure initial display page\n");
        return 1;
    }

    //Clear both pages in VRAM to black initially
    memset(vram_base_ptr, 0, vram_size);

    return 0;
}

int video_clear_page(void) {
    if (vram_base_ptr == NULL) {
        return 1;
    }
    // Clear only the inactive drawing page to avoid rendering flickering
    uint32_t page_offset = active_draw_page * page_size;
    memset(vram_base_ptr + page_offset, 0, page_size);
    return 0;
}

int video_swap_pages(void) {
    if (vram_base_ptr == NULL) {
        return 1;
    }

    uint16_t y_offset = active_draw_page * v_res;
    if (video_set_display_start(0, y_offset) != 0) {
        printf("[video_gr] failed to flip active page\n");
        return 1;
    }

    active_draw_page = 1 - active_draw_page;

    return 0;
}

int video_cleanup_page_flipping(void) {
    vram_base_ptr = NULL;
    vram_size = 0;
    page_size = 0;
    h_res = 0;
    v_res = 0;
    bytes_per_pixel = 0;
    active_draw_page = 1;
    return 0;
}

int video_draw_background(const uint8_t *background_pixmap) {
    if (vram_base_ptr == NULL || background_pixmap == NULL) {
        return 1;
    }
    uint32_t page_offset = active_draw_page * page_size;
    
    if (bytes_per_pixel == 3) {
        memcpy(vram_base_ptr + page_offset, background_pixmap, page_size);
    } else if (bytes_per_pixel == 4) {
        for (uint32_t i = 0; i < h_res * v_res; i++) {
            uint32_t xpm_offset = i * 3;
            uint32_t vram_offset = i * 4;
            
            uint32_t color = background_pixmap[xpm_offset] | 
                             (background_pixmap[xpm_offset + 1] << 8) | 
                             (background_pixmap[xpm_offset + 2] << 16);
            
            memcpy(vram_base_ptr + page_offset + vram_offset, &color, 4);
        }
    } else {
        return 1;
    }
    return 0;
}

int video_draw_pixel(uint16_t x, uint16_t y, uint32_t color) {
    if (vram_base_ptr == NULL) return 1;
    if (x >= h_res || y >= v_res) return 0; // Standard out-of-bounds clipping

    // Ignore chroma key green pixel (standard 24-bit background color used in place of transparency)
    if (bytes_per_pixel == 3 && (color & 0x00FFFFFF) == CHROMA_KEY_GREEN_888) return 0;

    // Calculate VRAM address based on the current inactive target drawing page
    uint32_t page_offset = active_draw_page * page_size;
    uint32_t pixel_offset = (y * h_res + x) * bytes_per_pixel;
    
    uint8_t *pixel_addr = vram_base_ptr + page_offset + pixel_offset;
    memcpy(pixel_addr, &color, bytes_per_pixel);

    return 0;
}

int video_draw_sprite(const uint8_t *pixmap, uint16_t width, uint16_t height, uint16_t x, uint16_t y) {
    if (vram_base_ptr == NULL || pixmap == NULL) return 1;

    // Bounds check at the sprite level
    if (x >= h_res || y >= v_res) return 0;

    uint32_t page_offset = active_draw_page * page_size;
    
    // We only draw the part of the sprite that fits on the screen (clipping)
    uint16_t draw_width = (x + width > h_res) ? (h_res - x) : width;
    uint16_t draw_height = (y + height > v_res) ? (v_res - y) : height;

    for (uint16_t row = 0; row < draw_height; row++) {
        // Calculate the exact destination address in VRAM for the start of this row
        uint32_t vram_offset = ((y + row) * h_res + x) * bytes_per_pixel;
        uint8_t *vram_ptr = vram_base_ptr + page_offset + vram_offset;
        
        // Calculate the start of this row in the source pixmap
        const uint8_t *xpm_ptr = pixmap + (row * width) * 3;

        for (uint16_t col = 0; col < draw_width; col++) {
            // Unpack 24-bit color from XPM
            uint32_t color = xpm_ptr[0] | (xpm_ptr[1] << 8) | (xpm_ptr[2] << 16);
            
            if ((color & 0x00FFFFFF) != CHROMA_KEY_GREEN_888) {
                if (bytes_per_pixel == 3) {
                    vram_ptr[0] = xpm_ptr[0];
                    vram_ptr[1] = xpm_ptr[1];
                    vram_ptr[2] = xpm_ptr[2];
                } else if (bytes_per_pixel == 4) {
                    *(uint32_t*)vram_ptr = color;
                }
            }
            vram_ptr += bytes_per_pixel;
            xpm_ptr += 3;
        }
    }
    return 0;
}
