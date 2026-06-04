#ifndef _GAME_OVER_DRAW_H_
#define _GAME_OVER_DRAW_H_

#include <stdint.h>
#include "../game/game_over.h"
#include "../init.h"

GameOverOption ui_game_over_hit_test(int x, int y);
int ui_game_over_draw(const game_over_t *game_over, const Assets *assets);

#endif /* _GAME_OVER_DRAW_H_ */
