#ifndef __MOUSE_H
#define __MOUSE_H

#include <stdint.h>


//subscribe to mouse interrupts and enable them
int (mouse_subscribe_int)(uint8_t *bit_no);
int (mouse_unsubscribe_int)(void);

int (mouse_read_byte)(uint8_t *byte); //read a byte from the mouse, with error checking
int (mouse_read_ack) (uint8_t *ack); //read an ACK byte from the mouse, with error checking
int (mouse_write_aux_command) (uint8_t command); //write a command to the mouse

int (mouse_enable_data_report)(void);
int (mouse_disable_data_report)(void);

int (mouse_packet_is_ready)();
struct packet* (handle_packet) ();

void (mouse_ih)(void);

//Getters for the data read from the mouse
bool (mouse_has_error)();
uint8_t (mouse_get_data)();

#endif /* __MOUSE_H */
