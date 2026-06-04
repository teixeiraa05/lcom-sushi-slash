#ifndef _SESSION_TIMER_H_
#define _SESSION_TIMER_H_

#include <stdbool.h>
#include <stdint.h>

typedef struct {
  uint32_t elapsed_ticks;
  int last_tick;
  bool running;
} GameSessionTimer;

void session_timer_reset(GameSessionTimer *timer, int current_tick);
void session_timer_start(GameSessionTimer *timer, int current_tick);
void session_timer_pause(GameSessionTimer *timer, int current_tick);
void session_timer_update(GameSessionTimer *timer, int current_tick);
uint32_t session_timer_seconds(const GameSessionTimer *timer);

#endif /* _SESSION_TIMER_H_ */
