#include <lcom/lcf.h>
#include "mouse.h"
#include "i8042.h"

int hook_id_mouse;


bool (input_buffer_free)() {
  uint8_t status;

  if (util_sys_inb(KBC_STAT_REG, &status)) return false; //if we can't read the status, assume the buffer is busy

  return (status & IBF) == 0; //input buffer is free if bit 1 of the status register is not set
}

bool (output_buffer_free)() {
  uint8_t status;
  if (util_sys_inb(KBC_STAT_REG, &status)) return false; 

  if(status & OBF) {
    if(status & (PARITY_ERR | TIMEOUT_ERR)) return false;
    return true; 
  }
  return false;
}

int (mouse_subscribe_int)(uint8_t *bit_no) {
  hook_id_mouse = MOUSE_IRQ;
  if(bit_no == NULL) return 1;
  *bit_no = hook_id_mouse;
  if(sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_mouse)) return 1;
  return 0;
}

int (mouse_unsubscribe_int)(void) {
  if(sys_irqrmpolicy(&hook_id_mouse)) return 1;
  return 0;
}

int (mouse_write) (uint8_t command) {
  if(!input_buffer_free()) return 1; //input buffer is busy, can't write to it
  if(sys_outb(KBC_CMD_REG, WRITE_TO_AUX)) return 1; //tell the KBC the byte is for the mouse

  if(!input_buffer_free()) return 1; //input buffer is busy, can't write to it
  if(sys_outb(KBC_IN_BUF, command)) return 1; //write the command to the mouse

  return 0;
}

int (mouse_enable_data_reporting)(void) {
  uint8_t response;

  if(mouse_write(ENABLE_DATA_REPORTING)) return 1; //send the command to enable data reporting

  if(!output_buffer_free()) return 1; //output buffer is busy, can't read the response
  if(util_sys_inb(KBC_OUT_BUF, &response)) return 1; //if response is not able return 1
  if(response != ACK_BYTE) return 1; //if the response is not ACK, return 1

  return 0;
}


void (mouse_ih)(void) {

}