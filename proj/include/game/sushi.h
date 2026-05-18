#ifndef __SUSHI_H
#define __SUSHI_H

#include "sprite.h"

typedef struct {
  float x, y; // Position of the sushi on the screen
  float vx, vy; // Velocity of the sushi
  bool active; // Whether the sushi is active (on screen) or not (to represent cut states)
  int score_value; // The score value of the sushi when cut

  sprite_t xpm; // The sprite representing the sushi
} sushi_t;

//LR fragments after cut

typedef struct {
  float x, y; // Position of the sushi fragment on the screen
  float vx, vy; // Velocity of the sushi fragment
  bool active; // Whether the sushi fragment is active (on screen) or not (to represent cut states)

  sprite_t xpm; // The sprite representing the sushi fragment
} sushi_fragment_t;

#endif /* __SUSHI_H */