#ifndef __TIMER_CONTROLLER_H
#define __TIMER_CONTROLLER_H

#include <stdint.h>
#include <stdbool.h>

/**
 * Timer Controller - Controla timing do jogo
 * Interface para o game loop e eventos baseados em tempo
 */

// Inicializa o timer
int timer_init(uint32_t frequency);

// Limpa o timer
int timer_cleanup(void);

// Retorna o número de interrupções ocorridas desde a última chamada
uint32_t timer_get_ticks(void);

// Reseta o contador de ticks
void timer_reset_ticks(void);

// Verifica se decorreu um certo tempo (em milisegundos)
bool timer_elapsed(uint32_t ms);

// Dorme durante um certo tempo (em milisegundos)
void timer_sleep(uint32_t ms);

#endif /* __TIMER_CONTROLLER_H */
