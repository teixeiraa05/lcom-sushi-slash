#ifndef __KBC_H
#define __KBC_H

#include <stdint.h>
#include <stdbool.h>

int(kbc_subscribe_int)(uint8_t *bit_no);

int(kbc_unsubscribe_int)();
 
uint8_t (kbc_get_scancode)();

uint8_t (kbc_has_error)();

#endif // __KBC_H
