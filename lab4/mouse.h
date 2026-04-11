#ifndef __MOUSE_H
#define __MOUSE_H

#include <stdint.h>

//check if buffers are busy before writing/reading - true if ready, false if busy
bool (input_buffer_free)();
bool (output_buffer_free)();

//subscribe to mouse interrupts and enable them
int (mouse_subscribe_int)(uint8_t *bit_no);
int (mouse_unsubscribe_int)(void);

void (mouse_ih)(void);
int (mouse_enable_data_reporting)(void);

#endif /* __MOUSE_H */