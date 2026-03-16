#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  /* To be implemented by the students */
  uint8_t st; 
  if (timer_get_conf(timer, &st)) return 1;
  st &= 0x0F; //keep only the 4 least significant bits
  st |= TIMER_LSB_MSB;
  st |= (TIMER_0 + timer) << 6;

  uint16_t div = TIMER_FREQ / freq;
  if(sys_outb(TIMER_CTRL, st)) return 1;

  uint8_t lsb, msb; 
  if(util_get_LSB(div, &lsb)) return 1;
  if(util_get_MSB(div, &msb)) return 1;

  if(sys_outb(TIMER_0 + timer, msb)) return 1;
  if(sys_outb(TIMER_0 + timer, lsb)) return 1;

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
    /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_unsubscribe_int)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

void (timer_int_handler)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  //build the read-back command
  uint8_t rb_cmd = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
  
  //send the read-back command to the control register
  if(sys_outb(TIMER_CTRL, rb_cmd)) return 1;
  
  //read the status byte from the selected timer port
  if(util_sys_inb(TIMER_0 + timer, st)) return 1;

  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  /* To be implemented by the students */
  union timer_status_field_val val;

  switch(field) {
    case tsf_all:
      val.byte = st;
      break;
    case tsf_initial:
      val.in_mode = INVAL_val;
      if (st & TIMER_LSB_MSB) val.in_mode = MSB_after_LSB;
      else if (st & TIMER_LSB) val.in_mode = LSB_only;
      else if (st & TIMER_MSB) val.in_mode = MSB_only;
      break;
    case tsf_mode:
      val.count_mode = (st & (BIT(3) | BIT(2) | BIT(1))) >> 1;
      if (val.count_mode == 6) val.count_mode = 2;
      else if (val.count_mode == 7) val.count_mode = 3;
      break;
    case tsf_base:
      val.bcd = st & TIMER_BCD;
      break;
    default:
      return 1;
  }


  return timer_print_config(timer, field, val);
}
