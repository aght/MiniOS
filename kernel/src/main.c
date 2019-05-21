#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "console/console.h"
#include "rpi/video/rpi_video.h"

static void delay(uint32_t count) {
    asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
                 : "=r"(count)
                 : [count] "0"(count)
                 : "cc");
}

void main(void) {
    rpi_video_init(640, 480, 24);
    hal_io_serial_init();
    ARM_setmaxspeed(NULL);
    sdInitCard(NULL, NULL, true);
    // console_init();
    // console_run();

    int xdir = 1;
    int ydir = 1;
    int x = 0;
    int y = 0;
    const int w = 230;
    const int h = 100;

    rgb_t red = {255, 0, 0};
    rgb_t green = {0, 255, 0};

    bool flip = true;

    while (1) {
        
        rpi_video_rect(x, y, w, h, green);

        x += xdir;
        y += ydir;

        if (x <= 0) {
            xdir *= -1;
        }

        if (x + w >= 640) {
            xdir *= -1;
        }

        if (y <= 0) {
            ydir *= -1;
        }

        if (y + h >= 480) {
            ydir *= -1;
        }

        // if (flip) {
        //     rpi_video_rect(x++, y++, w, h, red);
        // } else {
        //     rpi_video_rect(x++, y++, w, h, green);
        // }

        // flip = !flip;
        rpi_video_swap_buffer();
        rpi_video_clear();

        // delay(5000000);
    }
}
