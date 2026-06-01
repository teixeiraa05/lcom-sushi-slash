#include <lcom/lcf.h>
#include <string.h>

#include "../lab5/video_gr.h"
#include "../lab5/vesa-bios.h"
#include "../lab3/kbc.h"
#include "../lab3/i8042.h"
#include "../lab2/timer.h"
#include "include/macros/menu.h"
#include "include/ui/ui.h"
#include "include/game/sprite.h"
#include "include/init.h"
#include "include/ui/menu_draw.h"
#include "include/game/game.h"
#include "../lab4/mouse.h"

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
    if(video_init_page_flipping(MODE_115) != 0) {
         printf("Failed to initialize page-flipping\n");
         return 1;
     }
    
    // Load assets
    Assets assets;
    memset(&assets, 0, sizeof(assets));
    if (assets_init(&assets) != 0) {
        video_cleanup_page_flipping();
        return 1;
    }

    GameInfo game;
    memset(&game, 0, sizeof(game));
    game.state = MENU;
    menu_init(&game.data.menu);

        //Temporary stuff to test the video mode, can be removed later
    uint8_t kbc_bit_no;
    if(kbc_subscribe_int(&kbc_bit_no)) return 1;
    
    uint8_t timer_bit_no;
    if(timer_subscribe_int(&timer_bit_no)) {
        kbc_unsubscribe_int();
        return 1;
    }

    MouseGameState mouse_state;
    uint8_t mouse_bit_no;
    if (mouse_init(&mouse_state, SCREEN_WIDTH, SCREEN_HEIGHT, &mouse_bit_no) != 0) {
        timer_unsubscribe_int();
        kbc_unsubscribe_int();
        return 1;
    }

    uint32_t timer_irq_set = BIT(timer_bit_no);
    uint32_t kbc_irq_set = BIT(kbc_bit_no);
    uint32_t mouse_irq_set = BIT(mouse_bit_no);

    int r;  
    int ipc_status;
    message msg;

    uint8_t scancode[2] = {0};
    bool ready = keyboard_get_scancode(scancode);
    bool running = true;
    bool prev_lb_pressed = false;

    while(running) {

        /* Get a request message. */
        if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) { /* received notification */
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: /* hardware interrupt notification */
                    if (msg.m_notify.interrupts & kbc_irq_set) { /* subscribed interrupt */
                        keyboard_ih();
                        ready = keyboard_get_scancode(scancode);

                        if (ready && scancode[0] == ESC_BREAK_CODE && game.state == GAME) {
                            game.state = PAUSED;
                            pause_menu_init(&game.data.pause_menu);
                        }
                    }

                    if (msg.m_notify.interrupts & timer_irq_set) { /* subscribed interrupt */
                        timer_ih_proj();
                        
                        anim_update(&assets.mascot_anim, get_int_counter());
                        video_clear_page();
                        if (game.state == MENU) {
                            ui_menu_draw(&game.data.menu, &assets);
                        }
                        else if (game.state == GAME) {
                            ui_draw_background(&assets.background_sprite);
                        }
                        else if (game.state == PAUSED) {
                            ui_draw_background(&assets.background_sprite);
                            ui_pause_menu_draw(&game.data.pause_menu, &assets);
                        }
                        
                        if (game.state == MENU || game.state == GAME || game.state == PAUSED) {
                            ui_draw_sprite(&assets.mouse_cursor, mouse_state.x, mouse_state.y);
                        }

                        video_swap_pages();
                    }

                    if (msg.m_notify.interrupts & mouse_irq_set) { /* subscribed interrupt */
                        if (mouse_proj_ih(&mouse_state, SCREEN_WIDTH, SCREEN_HEIGHT)) {
                            bool clicked = mouse_state.lb_pressed && !prev_lb_pressed;
                            prev_lb_pressed = mouse_state.lb_pressed;

                            if (game.state == MENU) {
                                MenuOption hovered = ui_menu_hit_test(mouse_state.x, mouse_state.y);
                                MenuAction action = menu_update(&game.data.menu, hovered, clicked);

                                switch (action) {
                                    case MENU_ACTION_START_SINGLEPLAYER:
                                        game.state = GAME;
                                        break;
                                    case MENU_ACTION_START_MULTIPLAYER:
                                        game.state = GAME;
                                        break;
                                    case MENU_ACTION_SHOW_INSTRUCTIONS:
                                        break;
                                    case MENU_ACTION_EXIT:
                                        running = false;
                                        break;
                                    case MENU_ACTION_NONE:
                                    default:
                                        break;
                                }
                            }
                            else if (game.state == PAUSED) {
                                PauseOption hovered = ui_pause_menu_hit_test(mouse_state.x, mouse_state.y);
                                PauseAction action = pause_menu_update(&game.data.pause_menu, hovered, clicked);

                                switch (action) {
                                    case PAUSE_ACTION_RESUME:
                                        game.state = GAME;
                                        break;
                                    case PAUSE_ACTION_MAIN_MENU:
                                        game.state = MENU;
                                        menu_init(&game.data.menu);
                                        break;
                                    case PAUSE_ACTION_EXIT:
                                        running = false;
                                        break;
                                    case PAUSE_ACTION_NONE:
                                    default:
                                        break;
                                }
                            }
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
    
    if (timer_unsubscribe_int()) return 1;
    if(kbc_unsubscribe_int()) return 1;
    if (mouse_cleanup()) return 1;

    // Free any resources that were allocated
    assets_destroy(&assets);
    if(video_cleanup_page_flipping() != 0) return 1;
    if(vg_exit()) return 1;


    // --- END TODO -----------------------------
    return 0;
}
