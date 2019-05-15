#ifndef _SYSCALL_
#define _SYSCALL_

#include "syscall.h"

void *c_svc_handler(int svc_number, void *r1, void *r2, void *r3, void *r4) {
    switch (svc_number) {
        case 0: {
            switch ((char)r1) {
                case '\n':
                case '\r':
                    console_newline();
                    break;
                case '\b':
                    console_print("\b \b");
                    break;
                default:
                    console_print("%c", r1);
            }
        } break;
        case 1:
            return hal_io_serial_getc();
        case 2:
            console_clear();
        case 3:
            return getcwd(r1, r2);
        default:
            break;
    }
}

#endif
