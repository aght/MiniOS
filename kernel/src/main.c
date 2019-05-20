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
    // hal_io_serial_init();
    ARM_setmaxspeed(NULL);
    // sdInitCard(NULL, NULL, true);
    // console_init();
    // console_run();

    rgb_t red = {255, 0, 0};
    rgb_t green = {0, 255, 0};

    for (uint_fast32_t y = 0; y < 480; y++) {
        for (uint_fast32_t x = 0; x < 640; x++) {
            rpi_video_put_pixel(x, y, green);
        }
    }

    for (uint_fast32_t y = 320; y < 480 * 2; y++) {
        for (uint_fast32_t x = 0; x < 640; x++) {
            rpi_video_put_pixel(x, y, red);
        }
    }

    bool swap = true;
    while (1) {
        swap ? rpi_video_fill(red) : rpi_video_fill(green);

        rpi_video_swap_buffer();
        swap = !swap;
        for (long long i = 0; i < 100000000; i++)
            ;
    }
}
