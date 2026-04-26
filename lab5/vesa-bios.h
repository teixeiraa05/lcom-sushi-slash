#ifndef __VESA_BIOS_H
#define __VESA_BIOS_H

#define AH_VBE 0x4F
#define AL_SET_MODE 0x02
#define AL_GET_MODE 0x01
#define AL_GET_CONTROLLER_INFO 0x00
#define VBE_INT 0x10

#define CHECK_BIT_SET BIT(14) // Bit 14 must be set to indicate linear framebuffer mode

/* VBE MODES */
#define MODE_1 0x105 // 1024x768, 256 colors
#define MODE_2 0x110 // 640x480, 16-bit color
#define MODE_3 0x115 // 800x600, 24-bit color
#define MODE_4 0x11A // 1280x1024, 16-bit color
#define MODE_5 0x14C // 1152x864, 32-bit color

#endif /* __VESA_BIOS_H */
