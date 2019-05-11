#define APP_ENTRY __attribute__ ((section (".app_entry")))

#include "stdio/stdio.h"

APP_ENTRY int main() {
    printf("Hello from MiniApp!");

    return 0;
}
