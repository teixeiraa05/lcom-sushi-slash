#ifndef __KEYBOARD_CONTROLLER_H
#define __KEYBOARD_CONTROLLER_H

#include <stdint.h>
#include <stdbool.h>

/**
 * Keyboard Controller - Abstrai o teclado para o jogo
 * Interface limpa para receber input do teclado
 */

// Inicializa o sistema de teclado
int keyboard_init(void);

// Limpa o sistema de teclado
int keyboard_cleanup(void);

// Obtém a próxima tecla pressionada (scancode)
// Retorna 0 se nenhuma tecla foi pressionada
uint8_t keyboard_get_key(void);

// Verifica se uma tecla específica foi pressionada
bool keyboard_key_pressed(uint8_t scancode);


#endif /* __KEYBOARD_CONTROLLER_H */
