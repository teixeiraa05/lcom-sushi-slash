#ifndef __VIDEO_CONTROLLER_H
#define __VIDEO_CONTROLLER_H

#include <stdint.h>
#include "../../lab5/video_gr.h"

/**
 * Video Controller - Abstrai gráficos para o jogo
 * Interface simplificada para desenhar no ecrã
 */

// Inicializa o modo vídeo
// Retorna o endereço do buffer de vídeo
void* video_init(uint16_t mode);

// Limpa o modo vídeo e volta ao modo texto
int video_cleanup(void);

// Limpa o ecrã com uma cor
int video_clear(uint32_t color);

// Desenha um retângulo preenchido
int video_draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

// Desenha um XPM (imagem)
int video_draw_xpm(xpm_map_t xpm, uint16_t x, uint16_t y);

// Atualiza o ecrã (se necessário)
int video_update(void);

// Obtém as dimensões do ecrã
void video_get_resolution(uint16_t *width, uint16_t *height);

#endif /* __VIDEO_CONTROLLER_H */
