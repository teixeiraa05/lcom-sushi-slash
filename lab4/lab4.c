// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/lab4.h>

#include <stdint.h>
#include <stdio.h>

#include "mouse.h"
#include "i8042.h"

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (mouse_test_packet)(uint32_t cnt) {
  uint8_t bit_no;
  if (mouse_start_session(&bit_no)) return 1;

  uint32_t irq_set = BIT(bit_no);
  int byte_count = 0;
  uint8_t packet_bytes[PACKET_SIZE];
  struct packet p;

  int r;
  int ipc_status;
  message msg;

  while (cnt > 0) { //exit on ESC break code, but if it's a two byte scancode, we need to read both bytes before exiting

      /* Get a request message. */
      if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
          printf("driver_receive failed with: %d", r);
          continue;
      }
      if (is_ipc_notify(ipc_status)) { /* received notification */
          switch (_ENDPOINT_P(msg.m_source)) {
              case HARDWARE: /* hardware interrupt notification */
                  if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
                      mouse_ih();
                            if (mouse_has_error()) continue;

                      uint8_t byte = mouse_get_data();

                            if (byte_count == 0) {
                              if (!(byte & ALWAYS_1)) continue; // actually skip unsynced bytes
                          packet_bytes[0] = byte;
                          byte_count++;
                      } else {
                          packet_bytes[byte_count] = byte;
                          byte_count++;
                      }


                      if (byte_count == PACKET_SIZE) { //if we have read all three bytes of the packet
                        handle_packet(packet_bytes, &p);
                        mouse_print_packet(&p);
                        byte_count = 0; //reset for the next packet
                        cnt--; //decrease the packet counter
                      }
                    }
                  break;
              default:
                  break; /* no other notifications expected: do nothing */
          }
      } else { /* received a standard message, not a notification */
          /* no standard messages expected: do nothing */
      }
  }
  return mouse_stop_session();  
}

int (mouse_test_async)(uint8_t idle_time) {
    //TODO
    return 1;
}
