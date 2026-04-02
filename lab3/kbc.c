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
