#include <lcom/lcf.h>
#include <lab3/kbc.h>
#include <lab3/i8042.h>

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