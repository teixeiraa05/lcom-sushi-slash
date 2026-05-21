#include <lcom/lcf.h>

#include "../lab5/video_gr.h"
#include "../lab5/vesa-bios.h"



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

int proj_main_loop() {
    //--- TODO ----------------------------------
    // Set video mode
    if (set_video_mode(MODE_115) != 0) {
      vg_exit();
      return 1;
    }
    // Run the main game loop
    if (game_loop() != 0) {
      vg_exit();
      return 1;
    }
    // Exit video mode
    if(vg_exit() != 0) {
      return 1;
    }
    // Free any resources that were allocated


    // --- END TODO -----------------------------
    return 0;
}
