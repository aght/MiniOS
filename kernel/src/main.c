#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "console/console.h"
#include "drivers/sdcard/SDCard.h"
#include "drivers/stdio/emb-stdio.h"
#include "rpi/video/rpi_video.h"

void main(void) {
    rpi_video_init(640, 480, 24);
    hal_io_serial_init();
    ARM_setmaxspeed(NULL);
    sdInitCard(NULL, NULL, true);
    // console_init();
    // console_run();

    rgb_t red = {255, 0, 0};
    rgb_t green = {0, 255, 0};

    bool flip = true;

    while (1) {
        if (flip) {
            rpi_video_fill(green);
        } else {
            rpi_video_fill(red);
        }

        flip = !flip;
        rpi_video_swap_buffer();

        for (long long i = 0; i < 9000000; i++)
            ;
    }
}
