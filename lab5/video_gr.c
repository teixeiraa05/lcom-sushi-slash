#include <lcom/lcf.h>
#include <lcom/lab5.h>

#include "vesa-bios.h"
#include "video_gr.h"

void* (vg_init)(uint16_t mode) {
    reg86_t reg;
    memset(&reg, 0, sizeof(reg));

    reg.intno = VBE_INT;
    reg.ah = AH_VBE;
    reg.al = AL_SET_MODE;
    reg.bx = mode | BIT(14); 

    if (sys_int86(&reg) != OK) {  
        printf("Error calling sys_int86\n");
        return NULL;
    }

    // The linear frame buffer address is returned in the EAX register
    return (void*) reg.eax;
}
