#include <lcom/lcf.h>

// Include headers from previous labs
#include "../lab2/timer.h"
#include "../lab3/kbc.h"
#include "../lab3/i8042.h"
#include "../lab4/mouse.h"
#include "../lab5/video_gr.h"

#include <stdint.h>
#include <stdbool.h>

// Global variables for interrupt bit numbers
static uint8_t kbd_bit_no = 0;
static uint8_t mouse_bit_no = 0;
static uint8_t timer_bit_no = 0;

int (main)(int argc, char *argv[]) {

  lcf_set_language("EN-US");
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");
  lcf_log_output("/home/lcom/labs/proj/output.txt");
  if (lcf_start(argc, argv)) return 1;

  lcf_cleanup();

  return 0;
}

int (subscribe_interrupts)() {

  if (kbc_subscribe_int(&kbd_bit_no)) {
    return 1;
  }
  
  if (mouse_subscribe_int(&mouse_bit_no)) {
    kbc_unsubscribe_int();
    return 1;
  }
  
  if (timer_subscribe_int(&timer_bit_no)) {
    kbc_unsubscribe_int();
    mouse_unsubscribe_int();
    return 1;
  }

  if (vg_init(0x115) == NULL) {
    timer_unsubscribe_int();
    kbc_unsubscribe_int();
    mouse_unsubscribe_int();
    return 1;
  }
  
  return 0;
}


int (unsubscribe_interrupts)() {
  int ret = 0;
  
  if (kbc_unsubscribe_int()) {
    ret = 1;
  }
  
  if (mouse_unsubscribe_int()) {
    ret = 1;
  }
  
  if (timer_unsubscribe_int()) {
    ret = 1;
  }

  if (vg_exit()) {
    ret = 1;
  }
  
  return ret;
}

int (game_loop)() {

  while (true) {

    int r;  
    int ipc_status;
    message msg;

    if( driver_receive(ANY, &msg, &ipc_status) != 0 ){
        printf("Error");
        continue;
    }

    if(is_ipc_notify(ipc_status)) {
      switch(_ENDPOINT_P(msg.m_source)){
        case HARDWARE:
          //timer
          if (msg.m_notify.interrupts & irq_timer) {
            // Timer interrupt handling code
          }
          //Keyboard
          if (msg.m_notify.interrupts & irq_kb) {
            // Keyboard interrupt handling code
          }
          //mouse
          if (msg.m_notify.interrupts & irq_mouse) {
            // Mouse interrupt handling code
          }
          
        default:
          break;
      }
    }
  }
}

int (proj_main_loop)() {

  if(subscribe_interrupts()) return 1;

  if(game_loop()) return 1;

  if(unsubscribe_interrupts()) return 1;

  return 0;

}