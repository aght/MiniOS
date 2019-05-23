// https://github.com/BrianSidebotham/arm-tutorial-rpi

#ifndef _RPI_MAILBOX_H_
#define _RPI_MAILBOX_H_

#include "../rpi_base.h"

#define RPI_MAILBOX0_BASE (RPI_PERIPHERAL_BASE + 0xB880)

typedef enum {
    MB0_POWER_MANAGEMENT = 0,
    MB0_FRAMEBUFFER = 1,
    MB0_VIRTUAL_UART = 2,
    MB0_VCHIQ = 3,
    MB0_LEDS = 4,
    MB0_BUTTONS = 5,
    MB0_TOUCHSCREEN = 6,
    MB0_UNUSED = 7,
    MB0_TAGS_ARM_TO_VC = 8,
    MB0_TAGS_VC_TO_ARM = 9,
} mailbox0_channel_t;

enum mailbox_status_reg_bits {
    ARM_MS_FULL = 0x80000000,
    ARM_MS_EMPTY = 0x40000000,
    ARM_MS_LEVEL = 0x400000FF
};

typedef struct {
    volatile unsigned int read;
    volatile unsigned int res1[((0x90 - 0x80) / 4) - 1];
    volatile unsigned int poll;
    volatile unsigned int sender;
    volatile unsigned int status;
    volatile unsigned int config;
    volatile unsigned int write;
} mailbox_t;

void rpi_mailbox0write(mailbox0_channel_t channel, int data);
int rpi_mailbox0read(mailbox0_channel_t channel);

#endif
