#ifndef __RTC_PROJ_H
#define __RTC_PROJ_H

#include <stdint.h>

#include "../../lab1/rtc.h"

/**
 * @brief Represents date information read from the Real-Time Clock (RTC).
 * 
 * Conforms to LCOM styling guidelines (PascalCase naming).
 */
typedef struct {
    uint8_t day;   /**< The current day of the month (1-31). */
    uint8_t month; /**< The current month of the year (1-12). */
    uint8_t year;  /**< The current two-digit year (0-99). */
} RtcDate;

/**
 * @brief Initializes the Real-Time Clock (RTC) device controller.
 * 
 * @return 0 on success, non-zero on failure.
 */
int rtc_init(void);

/**
 * @brief Cleans up and releases the Real-Time Clock (RTC) resources.
 * 
 * @return 0 on success, non-zero on failure.
 */
int rtc_cleanup(void);

/**
 * @brief Reads the current system date from the RTC hardware.
 * 
 * Takes care of checking the Update-In-Progress (UIP) flag before reading
 * and handles any BCD-to-binary conversion.
 * 
 * @param date Pointer to the RtcDate structure to store the retrieved date.
 * @return 0 on success, non-zero on failure.
 */
int rtc_read_date(RtcDate *date);

#endif /* __RTC_PROJ_H */
