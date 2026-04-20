#include <lcom/lcf.h>
#include <lcom/lab5.h>

#include "vesa-bios.h"
#include "graphics-card.h"

int (graphics_card_change_mode) (uint16_t mode) {
    reg86_t reg;
    memset(&reg, 0, sizeof(reg));

    reg.intno = VBE_INT;
    reg.ah = AH_VBE;
    reg.al = AL_SET_MODE;
    reg.bx = mode | BIT(14); 


    if (sys_int86(&reg) != OK) {  
        printf("Error calling sys_int86\n");
        return 1;
    }

    return 0;
}
