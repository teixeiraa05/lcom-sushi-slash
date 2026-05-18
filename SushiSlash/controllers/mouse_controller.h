#ifndef __MOUSE_CONTROLLER_H
#define __MOUSE_CONTROLLER_H

#include <stdint.h>
#include <stdbool.h>

/**
 * Mouse Controller - Abstrai o rato para o jogo
 * Interface limpa para receber input do rato
 */

// Inicializa o sistema de rato
int mouse_init(void);

// Limpa o sistema de rato
int mouse_cleanup(void);

// Obtém o movimento do rato (dx, dy)
// Retorna true se houve movimento
bool mouse_get_movement(int *dx, int *dy);

// Obtém botões do rato pressionados
// Retorna máscara de botões (esquerdo, direito, meio)
uint8_t mouse_get_buttons(void);

// Obtém a posição atual do rato
void mouse_get_position(int *x, int *y);

#endif /* __MOUSE_CONTROLLER_H */
