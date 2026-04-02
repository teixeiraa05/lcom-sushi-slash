#include <lcom/lcf.h>
#include "kbc.h"
#include "i8042.h"

int hook_id_kb = 1;

int(kbc_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;
  *bit_no = hook_id_kb;
  if(sys_irqsetpolicy(KBC_IRQ, IRQ_ENABLE | IRQ_EXCLUSIVE, &hook_id_kb)) return 1;
  return 0;
}

int(kbc_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id_kb)) return 1;
  return 0;
}

static uint8_t scancode;
static bool kbc_error = false;

void (kbc_ih)() {
  uint8_t sr; 

  /* Read the status register */
  if (util_sys_inb(KBC_STAT_REG, &sr)) {
    kbc_error = true;
    return;
  }

  /* Check for errors, read the scancode even when we have errors, to discard it */
  if (sr & (PARITY_ERR | TIMEOUT_ERR)) {
    kbc_error = true;
  }

  /* Read the scancode from the output buffer */  
  if (util_sys_inb(KBC_OUT_BUF, &scancode)) {
    kbc_error = true;
    return;
  }
}

// ------------ GETTERS ------------

uint8_t (kbc_get_scancode)() {
  return scancode;
}

uint8_t (kbc_has_error)() {
  return kbc_error;
}

int (kbc_read_status)(uint8_t* status) {
  if (status == NULL) return 1; 
  if (util_sys_inb(KBC_STAT_REG, status)) return 1;
  return 0;
}

int (kbc_read_output)() {

  uint8_t status;

  if (kbc_read_status(&status)) return 1;

  //Check if output buffer is full. If so, we can read from it.
  if ((status & BIT(0))) {
      if(util_sys_inb(KBC_OUT_BUF, &scancode))   return 1;
      if((status & PARITY_ERR)) return 1;
      if((status & TIMEOUT_ERR)) return 1;
      return 0;
  }

  return 1;
}

int (kbc_read_command)(uint8_t *command) {

  uint8_t status;
  uint8_t attemps = 30;
  
  while (attemps) {

      if (kbc_read_status(&status)) return 1;

      //Check if output buffer is full. If so, we can read from it.
      if ((status & BIT(0))) {
          if(util_sys_inb(KBC_OUT_BUF, command))   return 1;
          if((status & PARITY_ERR)) return 1;
          if((status & TIMEOUT_ERR)) return 1;
          return 0;
      }

      tickdelay(micros_to_ticks(20000));
      attemps--;
  }

  return 1;
}

int (kbc_write_command)(uint8_t port, uint8_t commandByte) {

  uint8_t status;
  uint8_t attemps = 30;

  while (attemps) {

      if (kbc_read_status(&status) != 0) return 1;

      //Check if input buffer is empty. If so, we can read from it.
      if ((status & BIT(1)) == 0){
          if(sys_outb(port, commandByte)) return 1;
          return 0;
      }

      tickdelay(micros_to_ticks(20000));              
      attemps--;
  }

  return 1;
}

int (kbc_int_enable)() {

  //Reads the corrent command setting
  uint8_t commandWord;
  if (kbc_write_command(KBC_CMD_REG, 0x20)) return 1;
  if (kbc_read_command(&commandWord)) return 1;

  //Writes the enabling command
  commandWord = commandWord | BIT(0);
  if (kbc_write_command(KBC_CMD_REG, KBC_IN_BUF)) return 1;
  if (kbc_write_command(KBC_IN_BUF, commandWord)) return 1;

  return 0;
}
