#ifndef I8042_H
#define I8042_H

#define KBC_IRQ 1

#define KBC_OUT_BUF 0x60
#define KBC_IN_BUF 0x60

#define KBC_STAT_REG 0x64
#define KBC_CMD_REG 0x64

#define PARITY_ERR BIT(7)
#define TIMEOUT_ERR BIT(6)

#define ESC_BREAK_CODE 0x81

#define IBF BIT(1) //input buffer full
#define OBF BIT(0) //output buffer full

#endif //I8042_H
