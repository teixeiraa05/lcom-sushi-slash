#ifndef _MENU_DRAW_H_
#define _MENU_DRAW_H_

#include <stdint.h>
#include "../game/menu.h"
#include "../init.h"
#include "../../../lab1/rtc.h"

MenuOption ui_menu_hit_test(int x, int y);
PauseOption ui_pause_menu_hit_test(int x, int y);

int ui_menu_draw(const menu_t *menu, const Assets *assets, const rtc_date *date, const rtc_time_t *time, uint32_t fps);
int ui_pause_menu_draw(const pause_menu_t *menu, const Assets *assets, const rtc_date *date, const rtc_time_t *time, uint32_t fps);

#endif /* _MENU_DRAW_H_ */
