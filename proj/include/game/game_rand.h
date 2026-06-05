#ifndef _GAME_RAND_H_
#define _GAME_RAND_H_

#include <stdint.h>

#include "../../../lab1/rtc.h"
#include "../../../lab2/timer.h"

static inline uint32_t game_mix_seed(uint32_t seed, uint32_t value) {
    seed ^= value + 0x9E3779B9 + (seed << 6) + (seed >> 2);
    return seed;
}

static inline uint32_t game_rand(uint32_t *state) {
    *state = *state * 1103515245 + 12345;
    return (*state / 65536) % 32768;
}

static inline uint32_t game_seed_from_rtc(void) {
    rtc_date date = {0, 0, 0};
    rtc_time_t time = {0, 0, 0};
    uint32_t seed = 0x51A7B055;

    if (rtc_read_date(&date) == 0) {
        seed = game_mix_seed(seed, date.day);
        seed = game_mix_seed(seed, date.month);
        seed = game_mix_seed(seed, date.year);
    }

    if (rtc_read_time(&time) == 0) {
        seed = game_mix_seed(seed, time.hours);
        seed = game_mix_seed(seed, time.minutes);
        seed = game_mix_seed(seed, time.seconds);
    }

    seed = game_mix_seed(seed, (uint32_t)get_int_counter());
    return seed == 0 ? 1 : seed;
}

#endif /* _GAME_RAND_H_ */
