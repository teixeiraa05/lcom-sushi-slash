#include <lcom/lcf.h>

#include "../lab5/video_gr.h"
#include "../lab5/vesa-bios.h"
#include "../lab3/kbc.h"
#include "../lab3/i8042.h"
#include "../lab2/timer.h" 
#include "include/ui.h"
#include "include/game/sprite.h"
#include "include/game/init.h"

int main(int argc, char *argv[]) {
    // sets the language of LCF messages (can be either EN-US or PT-PT)
    lcf_set_language("EN-US");

    // enables to log function invocations that are being "wrapped" by LCF
    // [comment this out if you don't want/need it]
    lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

    // enables to save the output of printf function calls on a file
    // [comment this out if you don't want/need it]
    lcf_log_output("/home/lcom/labs/proj/output.txt");

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
    
    if(video_init_page_flipping(MODE_115) != 0) {
         printf("Failed to initialize page-flipping\n");
         return 1;
     }
    
    // Load assets
    Assets assets;
    memset(&assets, 0, sizeof(assets));
    if (assets_init(&assets) != 0) {
        assets_destroy(&assets);
        video_cleanup_page_flipping();
        return 1;
    }
    // --- END TODO -----------------------------

    //Temporary stuff to test the video mode, can be removed later
    uint8_t kbc_bit_no;
    if(kbc_subscribe_int(&kbc_bit_no)) return 1;
    
    uint8_t timer_bit_no;
    if(timer_subscribe_int(&timer_bit_no)) {
        kbc_unsubscribe_int();
        return 1;
    }

    uint32_t timer_irq_set = BIT(timer_bit_no);
    uint32_t kbc_irq_set = BIT(kbc_bit_no);

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
                    if (msg.m_notify.interrupts & kbc_irq_set) { /* subscribed interrupt */
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

                    if (msg.m_notify.interrupts & timer_irq_set) { /* subscribed interrupt */
                        timer_ih_proj();
                        
                        anim_update(&assets.mascot_anim, get_int_counter());
                        video_clear_page();
                        ui_draw_background(&assets.background_sprite);
                        ui_draw_sprite(&assets.test_sprite, 50, 100);
                        ui_draw_text(&assets.game_font, "play", 50, 50);
                        anim_draw(&assets.mascot_anim, 200, 200);

                        video_swap_pages();
                    }

                    break;
                default:
                    break; /* no other notifications expected: do nothing */
            }
        } else { /* received a standard message, not a notification */
            /* no standard messages expected: do nothing */
        }
    }
    
    if (timer_unsubscribe_int()) return 1;
    if(kbc_unsubscribe_int()) return 1;

    // Free any resources that were allocated
    assets_destroy(&assets);
    if(video_cleanup_page_flipping() != 0) return 1;
    if(vg_exit()) return 1;


    // --- END TODO -----------------------------
    return 0;
}
