#ifndef _RPI_MAILBOX_
#define _RPI_MAILBOX_

#include "rpi_mailbox.h"

static mailbox_t *mailbox0 = (mailbox_t *)RPI_MAILBOX0_BASE;

void rpi_mailbox0write(mailbox0_channel_t channel, int data) {
    data &= ~(0xf);
    data |= channel;

    while ((mailbox0->status & ARM_MS_FULL) != 0)
        ;

    mailbox0->write = data;
}

int rpi_mailbox0read(mailbox0_channel_t channel) {
    int data = -1;

    while ((data & 0xf) != channel) {
        while (mailbox0->status & ARM_MS_EMPTY)
            ;

        data = mailbox0->read;
    }

    return data >> 4;
}

#endif
