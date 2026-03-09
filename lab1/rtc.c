#include "rtc.h"
#include <minix/syslib.h>

#define TODO return -1

#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71
#define RTC_REG_A 0x0A
#define RTC_REG_B 0x0B
#define RTC_REG_DAY 0x07
#define RTC_REG_MONTH 0x08
#define RTC_REG_YEAR 0x09
#define RTC_UIP_MSK (1 << 7)
#define RTC_DM_MSK (1 << 2)

uint8_t bcd_to_bin(uint8_t bcd) { 
  return ((bcd >> 4)*10) + (bcd & 0x0F);
}

int rtc_read_date(rtc_date *date) { 

  uint32_t uipA;
  do {
    if (sys_outb(RTC_ADDR_REG, RTC_REG_A)) return 1; 
    if (sys_inb(RTC_DATA_REG, &uipA)) return 1;
  } while (uipA & RTC_UIP_MSK);

  uint32_t day;
  if (sys_outb(RTC_ADDR_REG, RTC_REG_DAY)) return 1; 
  if (sys_inb(RTC_DATA_REG, &day)) return 1;

  uint32_t month;
  if (sys_outb(RTC_ADDR_REG, RTC_REG_MONTH)) return 1; 
  if (sys_inb(RTC_DATA_REG, &month)) return 1;

  uint32_t year;
  if (sys_outb(RTC_ADDR_REG, RTC_REG_YEAR)) return 1;
  if (sys_inb(RTC_DATA_REG, &year)) return 1;

  uint32_t uipB;
  if (sys_outb(RTC_ADDR_REG, RTC_REG_B)) return 1; 
  if (sys_inb(RTC_DATA_REG, &uipB)) return 1;
  if (!(uipB & RTC_DM_MSK)) {
    day = bcd_to_bin((uint8_t)day);
    month = bcd_to_bin((uint8_t)month);
    year = bcd_to_bin((uint8_t)year);
  }

  date->day = day;
  date->month = month;
  date->year = year;

  return 0;
}
