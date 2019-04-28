#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "console/console.h"
#include "drivers/sdcard/SDCard.h"
#include "drivers/stdio/emb-stdio.h"
#include "hal/hal.h"

void main(void) {
    hal_io_video_init();
    hal_io_serial_init();
    sdInitCard(NULL, NULL, true);
    console_init();

    console_run();
}
