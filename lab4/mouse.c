#include <lcom/lcf.h>
#include "mouse.h"
#include "../lab3/kbc.h"
#include "i8042.h"

// ============ STATIC STATE ============
static bool mouse_error = false;
static int hook_id_mouse;
static uint8_t data;
static uint8_t packet_bytes[PACKET_SIZE];
static int byte_count = 0;
static struct packet p;

// ============ INTERRUPT SUBSCRIPTION ============
int mouse_write_aux_command(uint8_t command) {
  if (kbc_write_command(KBC_CMD_REG, WRITE_TO_AUX)) return 1;
  if (kbc_write_command(KBC_IN_BUF, command)) return 1;
  return 0;
}

int (mouse_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;
  hook_id_mouse = MOUSE_IRQ; 
  *bit_no = hook_id_mouse;
  if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_mouse)) return 1;

  return 0;
}

int (mouse_unsubscribe_int)(void) {
  if (sys_irqrmpolicy(&hook_id_mouse)) return 1;

  return 0;
}

// ============ COMMAND & ACK HANDLING ============

// Helper: read a single byte from KBC output buffer if mouse data is available
// Returns 0 on success (byte stored in *byte), 1 on failure (not ready or error)
int mouse_read_byte(uint8_t *byte) {
  if (byte == NULL) return 1;

  uint8_t status = 0;
  if (kbc_read_status(&status)) return 1;

  if ((status & OBF) && (status & MOUSE_DATA)) {
    if (status & (PARITY_ERR | TIMEOUT_ERR)) return 1;
    if (util_sys_inb(KBC_OUT_BUF, byte)) return 1;
    return 0;
  }

  return 1;
}

int mouse_read_ack(uint8_t *ack) {
  if (ack == NULL) return 1;

  for (int wait = 0; wait < KBC_MAX_ATTEMPTS; wait++) {
    if (mouse_read_byte(ack) == 0) return 0;
    tickdelay(micros_to_ticks(MOUSE_RESPONSE_DELAY));
  }

  return 1;
}

int (mouse_change_data_report)(uint8_t command) {
  uint8_t response = 0;

  for (int attempt = 0; attempt < KBC_MAX_ATTEMPTS; attempt++) {
    if (mouse_write_aux_command(command)) return 1;

    if (mouse_read_ack(&response)) continue; // timeout/status failure, resend command
    if (response == ACK_BYTE) return 0;
    if (response == NACK_BYTE || response == ERROR_BYTE) continue;
  }

  return 1;
}

int (mouse_enable_data_report)(void) {
  return mouse_change_data_report(ENABLE_DATA_REPORTING);
}

int (mouse_disable_data_report)(void) {
  return mouse_change_data_report(DISABLE_DATA_REPORTING);
}


// ============ PACKET HANDLING ============


int (mouse_packet_is_ready)() {
  if (byte_count == 0) {
    if (!(data & ALWAYS_1)) return 0; // actually skip unsynced bytes
    packet_bytes[0] = data;
    byte_count++;
    return 0;
  } else {
    packet_bytes[byte_count] = data;
    byte_count++;
    if (byte_count == PACKET_SIZE) {
      byte_count = 0; // reset for next packet
      return 1; // packet is ready
    }
    return 0; // packet not ready yet
  }
}

struct packet* (handle_packet) () {
  p.bytes[0] = packet_bytes[0];
  p.bytes[1] = packet_bytes[1];
  p.bytes[2] = packet_bytes[2];

  p.lb = (packet_bytes[0] & LB) != 0;
  p.mb = (packet_bytes[0] & MB) != 0;
  p.rb = (packet_bytes[0] & RB) != 0;

  p.delta_x = (packet_bytes[0] & X_SIGN) ? (packet_bytes[1] - 256) : packet_bytes[1]; 
  p.delta_y = (packet_bytes[0] & Y_SIGN) ? (packet_bytes[2] - 256) : packet_bytes[2]; 

  p.x_ov = (packet_bytes[0] & X_OVF) != 0;
  p.y_ov = (packet_bytes[0] & Y_OVF) != 0;

  return &p;
}

// ============ INTERRUPT HANDLER ============

void (mouse_ih)(void) {
  mouse_error = false;

  if (mouse_read_byte(&data) == 0) {
    return; // success, byte is in &data
  }

  // Read failed — check if we need to drain buffer due to error
  uint8_t status;
  if (kbc_read_status(&status) == 0) {
    if ((status & (PARITY_ERR | TIMEOUT_ERR)) && (status & OBF)) {
      util_sys_inb(KBC_OUT_BUF, &data); // drain stale data
    }
  }

  mouse_error = true;
}

// ============ GETTERS ============
bool (mouse_has_error)() {
  return mouse_error;
}

uint8_t (mouse_get_data)() {
  return data;
}

