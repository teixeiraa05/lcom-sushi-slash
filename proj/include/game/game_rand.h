#ifndef _GAME_RAND_H_
#define _GAME_RAND_H_

#include <stdint.h>


static inline uint32_t game_rand(uint32_t *state) {
    *state = *state * 1103515245 + 12345;
    return (*state / 65536) % 32768;
}

#endif /* _GAME_RAND_H_ */
