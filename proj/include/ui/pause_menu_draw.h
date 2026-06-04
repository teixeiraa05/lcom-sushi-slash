#ifndef _PAUSE_MENU_DRAW_H_
#define _PAUSE_MENU_DRAW_H_

#include <stdint.h>
#include "../game/pause_menu_state.h"
#include "../init.h"
#include "status_bar.h"

PauseOption ui_pause_menu_hit_test(int x, int y);
int ui_pause_menu_draw(const pause_menu_t *menu, const Assets *assets, const StatusBarInfo *status);

#endif /* _PAUSE_MENU_DRAW_H_ */
