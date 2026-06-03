#ifndef __SUSHI_H
#define __SUSHI_H

#include <stdlib.h>
#include "../macros/global.h"
#include "sprite.h"

#define SUSHI_SPAWN_Y_OFFSET 80
#define SUSHI_GRAVITY 1
#define SUSHI_SCORE_VALUE 1

typedef struct {
  int x, y; // Position of the sushi on the screen
  int vx, vy; // Velocity of the sushi
  int score_value; // The score value of the sushi when cut
  uint8_t type; // Index of the sushi XPM

  sprite_t* xpm; // The sprite representing the sushi
} sushi_t;

//LR fragments after cut

sushi_t create_sushi(int x, int y, int vx, int vy, sprite_t* xpm, int score_value);

sushi_t create_random_sushi(int speed);

void update_sushi(sushi_t* sushi);

void sushi_destroy(sushi_t* sushi);

typedef struct {
  int x, y; // Position of the sushi fragment on the screen
  int vx, vy; // Velocity of the sushi fragment

  sprite_t* xpm; // The sprite representing the sushi fragment
} sushi_fragment_t;

int find_cut_sushi(sushi_t* sushi, int cut_x, int cut_y, sushi_fragment_t* left_fragment, sushi_fragment_t* right_fragment);

int find_cut_sushi_area(sushi_t* sushi, int cut_x, int cut_y, int radius, sushi_fragment_t* left_fragment, sushi_fragment_t* right_fragment);

int find_cut_sushi_segment_area(sushi_t* sushi, int cut_x0, int cut_y0, int cut_x1, int cut_y1, int radius, sushi_fragment_t* left_fragment, sushi_fragment_t* right_fragment);

int cut_sushi(sushi_t* sushi, sushi_fragment_t* left_fragment, sushi_fragment_t* right_fragment);

void update_sushi_fragment(sushi_fragment_t* fragment);

void sushi_fragment_destroy(sushi_fragment_t* fragment);

#endif /* __SUSHI_H */
