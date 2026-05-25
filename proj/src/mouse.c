#include <lcom/lcf.h>
#include <stdint.h>
#include <string.h>

#include "../include/devices/mouse.h"
#include "../../lab4/i8042.h"

int mouse_init(MouseGameState *state, int screen_width, int screen_height, uint16_t *irq_set) {
    if (state == NULL || irq_set == NULL) {
        return 1;
    }

    // Initialize state
    memset(state, 0, sizeof(MouseGameState));
    state->x = screen_width / 2; // Start in the middle of the screen
    state->y = screen_height / 2;

    // Subscribe to mouse interrupts
    if (mouse_subscribe_int(irq_set) != 0) {
        printf("[mouse] Failed to subscribe to mouse interrupts\n");
        return 1;
    }

    // Enable data reporting
    if (mouse_enable_data_reporting() != 0) {
        printf("[mouse] Failed to enable data reporting\n");
        mouse_unsubscribe_int();
        return 1;
    }

    return 0;
}

int mouse_cleanup(void) {

  int ret_value = 0;
  if (mouse_disable_data_report() != 0) {
      printf("[mouse] Failed to disable data reporting\n");
      ret_value = 1;
    }

  if (mouse_unsubscribe_int() != 0) {
      printf("[mouse] Failed to unsubscribe from mouse interrupts\n");
      ret_value = 1;
    }

  return ret_value;

}

bool mouse_ih(MouseGameState *state, int screen_width, int screen_height) {
    if (state == NULL) {
        return false;
    }

    // Call low-level handler to read byte and compile packet
    mouse_ih(); // This updates the static packet 'p' when a full packet is ready

    // Check if a full packet was parsed
    if (mouse_packet_is_ready()) {
        struct packet *pkt = handle_packet();

        mouse_update_state(&state, &pkt, screen_width, screen_height);
        
        return true; // Packet processed and state updated
    }

    return false; // No complete packet yet
}

void mouse_update_state(MouseGameState *state, struct packet *pp, int screen_width, int screen_height) {
    // Update button states
    state->lb_pressed = pp->lb;
    state->mb_pressed = pp->mb;
    state->rb_pressed = pp->rb;

    // Update relative movement deltas
    state->dx = pp->delta_x;
    state->dy = screen_height - pp->delta_y; // Invert Y-axis

    // Update absolute position with clamping
    state->x += state->dx;
    state->y += state->dy;

    if (state->x < 0) state->x = 0;
    if (state->x > screen_width) state->x = screen_width;
    if (state->y < 0) state->y = 0;
    if (state->y > screen_height) state->y = screen_height;

    // Detect slash gesture
    int velocity_squared = state->dx * state->dx + state->dy * state->dy;
    const int SLASH_THRESHOLD_SQUARED = 10000; // Example threshold for slash detection
    state->is_slashing = (velocity_squared > SLASH_THRESHOLD_SQUARED) && state->lb_pressed;
}