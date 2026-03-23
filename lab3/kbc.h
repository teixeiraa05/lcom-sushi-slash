#ifndef __KBC_H
#define __KBC_H

#include <stdint.h>
#include <stdbool.h>

int(kbc_subscribe_int)(uint8_t *bit_no);
int(kbc_unsubscribe_int)();
 

#endif // __KBC_H