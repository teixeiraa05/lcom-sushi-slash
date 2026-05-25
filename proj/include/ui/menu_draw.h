#ifndef _MENU_DRAW_H_
#define _MENU_DRAW_H_

#include <stdint.h>
#include "../game/menu.h"
#include "../init.h"

MenuOption ui_menu_hit_test(int x, int y);
PauseOption ui_pause_menu_hit_test(int x, int y);

int ui_menu_draw(const menu_t *menu, const Assets *assets);
int ui_pause_menu_draw(const pause_menu_t *menu, const Assets *assets);

#endif /* _MENU_DRAW_H_ */
