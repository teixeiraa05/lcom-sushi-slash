#ifndef __RTC_CONTROLLER_H
#define __RTC_CONTROLLER_H

#include <stdint.h>

/**
 * RTC Controller - Abstrai o RTC (Real Time Clock) para o jogo
 * Interface para leitura da data/hora do sistema
 */

typedef struct {
    uint8_t day;
    uint8_t month;
    uint8_t year;
} rtc_date_t;

// Inicializa o RTC
int rtc_init(void);

// Limpa o RTC
int rtc_cleanup(void);

// Lê a data atual do RTC
// Retorna 0 em sucesso, non-zero em erro
int rtc_read_date(rtc_date_t *date);

#endif /* __RTC_CONTROLLER_H */
