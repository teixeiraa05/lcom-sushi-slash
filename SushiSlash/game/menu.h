#ifndef __MENU_H
#define __MENU_H

#include "../../lab5/video_gr.h"
#include "selected.h"

typedef struct menu {
    selected_t selected;
    xpm_map_t xpm;
} menu_t;

#endif /* __MENU_H */
