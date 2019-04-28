#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "drivers/stdio/emb-stdio.h"
#include "drivers/sdcard/SDCard.h"
#include "hal/hal.h"

void main(uint32_t r0, uint32_t r1, uint32_t atags)
{
    hal_io_video_init();
    hal_io_serial_init();
    sdInitCard (NULL, NULL, true);

    printf("Hello, World!\n");
}
