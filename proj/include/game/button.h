#ifndef __BUTTON_H
#define __BUTTON_H

#include <stdint.h>

typedef struct {
  int x, y; // Position of the button on the screen
  int width, height; // Dimensions of the button
  uint32_t color; // Color of the button 
  uint32_t hover_color; // Color of the button when hovered
  char* label; // Text label of the button
  bool hovered; // Whether the button is currently hovered
} button_t;

#endif /* __BUTTON_H */
