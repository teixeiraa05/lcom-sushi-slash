#include "../../include/ui/session_timer.h"

#include <stdio.h>

#include "../../include/macros/global.h"
#include "../../include/macros/menu.h"

#define SESSION_TIMER_TICKS_PER_SECOND 60
#define SESSION_TIMER_Y 10

void session_timer_reset(GameSessionTimer *timer, int current_tick) {
    if (timer == NULL) return;

    timer->elapsed_ticks = 0;
    timer->last_tick = current_tick;
    timer->running = false;
}

void session_timer_start(GameSessionTimer *timer, int current_tick) {
    if (timer == NULL) return;

    timer->last_tick = current_tick;
    timer->running = true;
}

void session_timer_pause(GameSessionTimer *timer, int current_tick) {
    if (timer == NULL) return;

    session_timer_update(timer, current_tick);
    timer->running = false;
}

void session_timer_update(GameSessionTimer *timer, int current_tick) {
    if (timer == NULL || !timer->running) return;

    if (current_tick > timer->last_tick) {
        timer->elapsed_ticks += (uint32_t)(current_tick - timer->last_tick);
    }

    timer->last_tick = current_tick;
}

uint32_t session_timer_seconds(const GameSessionTimer *timer) {
    if (timer == NULL) return 0;

    return timer->elapsed_ticks / SESSION_TIMER_TICKS_PER_SECOND;
}

int ui_session_timer_draw(const Assets *assets, const GameSessionTimer *timer) {
    if (assets == NULL || timer == NULL) return 1;

    uint32_t total_seconds = session_timer_seconds(timer);
    uint32_t minutes = total_seconds / 60;
    uint32_t seconds = total_seconds % 60;
    char timer_text[16];

    snprintf(timer_text, sizeof(timer_text), "time %02u:%02u",
             (unsigned)minutes,
             (unsigned)seconds);

    uint16_t text_width = font_text_width(&assets->game_font, timer_text);
    uint16_t x = CENTER_X(text_width);

    return font_draw_scaled_text_color(&assets->game_font,
                                       timer_text,
                                       x,
                                       SESSION_TIMER_Y,
                                       1,
                                       MENU_STATUS_COLOR);
}
