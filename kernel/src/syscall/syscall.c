#ifndef _SYSCALL_
#define _SYSCALL_

#include "syscall.h"

void *c_svc_handler(int svc_number, void *r1, void *r2, void *r3, void *r4) {
    switch (svc_number) {
        case 0:
            return console_print("%c", r1);
        case 1:
            return hal_io_serial_getc();
        default:
            break;
    }
}

#endif
