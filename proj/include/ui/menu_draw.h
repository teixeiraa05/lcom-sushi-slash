#ifndef _MENU_DRAW_H_
#define _MENU_DRAW_H_

#include <stdint.h>
#include "../game/menu_state.h"
#include "../init.h"
#include "status_bar.h"

MenuOption ui_menu_hit_test(int x, int y);
int ui_menu_draw(const menu_t *menu, const Assets *assets, const StatusBarInfo *status);

#endif /* _MENU_DRAW_H_ */
