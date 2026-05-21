#ifndef __TIMER_PROJ_H
#define __TIMER_PROJ_H

#include <stdint.h>
#include <stdbool.h>

#include "../../lab2/timer.h"
#include "../../lab2/i8254.h"


#define TIMER_FREQ_HZ 60 // Desired timer frequency in Hz for game ticks

/**
 * @brief Initializes the timer, subscribes to interrupts.
 * 
 * @param irq_set Pointer to store the subscribed IRQ hook bitmask.
 * @return 0 on success, non-zero on failure.
 */
int timer_init( uint16_t *irq_set);

/**
 * @brief Unsubscribes from timer interrupts and cleans up resource state.
 * 
 * @return 0 on success, non-zero on failure.
 */
int timer_cleanup(void);

/**
 * @brief Project-level timer interrupt handler.
 * 
 * Increments the global game tick counter.
 */
void timer_ih(void);

/**
 * @brief Retrieves the current tick count accumulated since the last reset.
 * 
 * @return The accumulated tick count.
 */
uint32_t timer_get_ticks(void);

/**
 * @brief Checks if a specific duration (in milliseconds) has elapsed.
 * 
 * Uses delta-tick comparisons based on TIMER_FREQ_HZ to evaluate elapsed time.
 * 
 * @param ms The duration to check in milliseconds.
 * @return true if the duration has elapsed, false otherwise.
 */
bool timer_elapsed(uint32_t ms);

/**
 * @brief Pauses execution of the program for a specific duration in milliseconds.
 * 
 * Blocks execution by waiting on the timer tick counter.
 * 
 * @param ms The duration to sleep in milliseconds.
 */
void timer_sleep(uint32_t ms);

#endif /* __TIMER_PROJ_H */
