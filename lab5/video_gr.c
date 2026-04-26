#include <lcom/lcf.h>
#include <lcom/lab5.h>

#include "vesa-bios.h"
#include "video_gr.h"

int (set_video_mode)(uint16_t mode) {

    reg86_t reg;
    memset(&reg, 0, sizeof(reg));
    
    reg.intno = VBE_INT;
    reg.ah = AH_VBE;
    reg.al = AL_SET_MODE;
    
    reg.bx = CHECK_BIT_SET | mode;
    
    if(sys_int86(&reg) != OK ) { 
        printf("sys_int86() failed \n");
        return 1;
    }
    
    return 0;
}

static char *video_mem;

static vbe_mode_info_t vmi;
static unsigned bytes_per_pixel;


void* (vg_init)(uint16_t mode) {

    memset(&vmi, 0, sizeof(vmi));
    if (vbe_get_mode_info(mode, &vmi)) return NULL;
    
    
    bytes_per_pixel = (vmi.BitsPerPixel + 7) / 8;
    
    int r;
    struct minix_mem_range mr;

    /* Need to allocate memory for the video mode */
    unsigned int vram_base = vmi.PhysBasePtr;
    unsigned int vram_size = vmi.XResolution * vmi.YResolution * bytes_per_pixel;

    mr.mr_base = (phys_bytes) vram_base;
    mr.mr_limit = mr.mr_base + vram_size;

    /* Allow memory mapping */
    if( (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)) != OK ) {
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);
        return NULL;
    }

    /* Map memory */
    video_mem = vm_map_phys(SELF, (void*)mr.mr_base, vram_size);

    if(video_mem == MAP_FAILED) {
        panic("couldn't map video memory");
        return NULL;
    }

    if (set_video_mode(mode)) return NULL;

    return video_mem;
}

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
    if(x < 0 || y < 0) return 1;
    if (x >= vmi.XResolution || y >= vmi.YResolution) return 1;

    //Handle mode 0x110 where the color is represented in 15 bits
    if(vmi.BitsPerPixel == 15) color &= 0x7FFF;

    //calculate the offset in the video memory for the pixel at (x, y)
    uint32_t pixel_offset = (y * vmi.XResolution + x) * bytes_per_pixel;
    char* pixel_addr = video_mem + pixel_offset;

    if (memcpy(pixel_addr, &color, bytes_per_pixel) == NULL) return 1;
    
    return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
    if(x < 0 || y < 0) return 1;
    if (x + len > vmi.XResolution || y >= vmi.YResolution) return 1;

    for (uint16_t i = 0; i < len; i++) {
        if (vg_draw_pixel(x + i, y, color)) return 1;
    }

    return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
    if(x < 0 || y < 0) return 1;
    if (x + width > vmi.XResolution || y + height > vmi.YResolution) return 1;

    for (uint16_t j = 0; j < height; j++) {
        if (vg_draw_hline(x, y + j, width, color)) return 1;
    }

    return 0;
}
