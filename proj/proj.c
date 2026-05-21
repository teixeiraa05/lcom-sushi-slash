#include <lcom/lcf.h>

#include "../lab5/video_gr.h"
#include "../lab5/vesa-bios.h"
#include "../lab3/kbc.h"
#include "../lab3/i8042.h"
#include "include/devices/video_gr.h"


int main(int argc, char *argv[]) {
    // sets the language of LCF messages (can be either EN-US or PT-PT)
    lcf_set_language("EN-US");

    // enables to log function invocations that are being "wrapped" by LCF
    // [comment this out if you don't want/need it]
    lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

    // enables to save the output of printf function calls on a file
    // [comment this out if you don't want/need it]
    lcf_log_output("/home/lcom/labs/lab5/output.txt");

    // handles control over to LCF
    // [LCF handles command line arguments and invokes the right function]
    if (lcf_start(argc, argv))
        return 1;

    // LCF clean up tasks
    // [must be the last statement before return]
    lcf_cleanup();

    return 0;
}

int (proj_main_loop)(int argc, char *argv[]) {
    //--- TODO ----------------------------------
    // Set video mode
    
    if(graphics_init(MODE_115) == NULL) {
         printf("Failed to initialize graphics mode\n");
         return 1;
     }
    
    //----- KEYBOARD (this is temporary and used just for closing the program) -----

    uint8_t bit_no;
    if(kbc_subscribe_int(&bit_no)) return 1;

    uint32_t irq_set = BIT(bit_no);

    int r;  
    int ipc_status;
    message msg;

    //Variables to read scancodes
    bool is_two_byte = false;
    uint8_t scancode[2] = {0};
    int scancode_size = 0; //for the print_scancode function

    while(scancode[0] != ESC_BREAK_CODE) { //exit on ESC break code, but if it's a two byte scancode, we need to read both bytes before exiting

        /* Get a request message. */
        if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) { /* received notification */
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: /* hardware interrupt notification */
                    if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
                        kbc_ih();

                        /* If there was an error, skip the scancode processing and wait for the next interrupt */
                        if(kbc_has_error()) {
                            continue;
                        }

                        /* Process the scancode */
                        if(is_two_byte) {
                            scancode[1] = kbc_get_scancode();
                            scancode_size = 2;
                            is_two_byte = false; //reset for the next scancode
                        }

                        else {
                            scancode[0] = kbc_get_scancode();
                        
                            if (scancode[0] == 0xE0) { //first byte of a two-byte scancode
                                is_two_byte = true;
                                continue; //wait for the next interrupt to read the second byte
                            }

                            scancode_size = 1;
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
    if(vg_exit()) return 1;

    if(kbc_unsubscribe_int()) return 1;
    // Free any resources that were allocated


    // --- END TODO -----------------------------
    return 0;
}
