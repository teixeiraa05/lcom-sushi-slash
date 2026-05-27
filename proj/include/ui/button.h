#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct {
  uint16_t x, y; // Position of the button on the screen
  uint16_t width, height; // Dimensions of the button
  uint32_t color; // Color of the button 
  uint32_t hover_color; // Color of the button when hovered
  const char *label; // Text label of the button
  bool hovered; // Whether the button is currently hovered
} button_t;

#endif /* _BUTTON_H_ */
