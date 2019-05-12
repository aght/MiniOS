#define APP_ENTRY __attribute__((section(".app_entry")))

#include "stdio/stdio.h"

APP_ENTRY int main() {
    printf("Typewriter!\n");
    printf("Use CTRL-D to exit\n");

    while (1) {
        char c = getc();

        // CTRL-D
        if (c == 4) {
            break;
        }

        printf("%c", c);
    }

    return 0;
}
