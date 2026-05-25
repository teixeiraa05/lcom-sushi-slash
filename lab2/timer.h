#ifndef _LAB2_TIMER_H_
#define _LAB2_TIMER_H_

#include <stdint.h>
#include <stdbool.h>

int get_int_counter();
int set_int_counter(int new_value);

// Project timer functions
void timer_ih_proj(void);
uint32_t timer_get_ticks(void);
bool timer_elapsed(int ms);

#endif /* _LAB2_TIMER_H_ */
