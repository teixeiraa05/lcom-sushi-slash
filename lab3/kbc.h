#ifndef __KBC_H
#define __KBC_H

#include <stdint.h>
#include <stdbool.h>

int(kbc_subscribe_int)(uint8_t *bit_no);

int(kbc_unsubscribe_int)();
 
uint8_t (kbc_get_scancode)();

uint8_t (kbc_has_error)();

int (kbc_read_status)(uint8_t* status);

int (kbc_read_output)();

int (kbc_read_command)(uint8_t *command);

int (kbc_write_command)(uint8_t port, uint8_t commandByte);

int (kbc_int_enable)();

#endif // __KBC_H
