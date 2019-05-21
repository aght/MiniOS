#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "console/console.h"
#include "rpi/video/rpi_video.h"

typedef struct {
    int x;
    int y;
    int w;
    int h;
    int xdir;
    int ydir;
} rect;

void update_rect(rect* rect) {
    rect->x += rect->xdir;
    rect->y += rect->ydir;

    if (rect->x <= 0) {
        rect->xdir *= -1;
    }

    if (rect->x + rect->w >= 640) {
        rect->xdir *= -1;
    }

    if (rect->y <= 0) {
        rect->ydir *= -1;
    }

    if (rect->y + rect->h >= 480) {
        rect->ydir *= -1;
    }
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

    rect rect1 = {0, 0, 230, 100, 1, 1};
    rect rect2 = {50, 100, 50, 60, -1, 1};

    rgb_t red = {255, 0, 0};
    rgb_t green = {0, 255, 0};

    while (1) {
        rpi_video_rect(rect1.x, rect1.y, rect1.w, rect1.h, green);
        rpi_video_rect(rect2.x, rect2.y, rect2.w, rect2.h, red);

        update_rect(&rect1);
        update_rect(&rect2);

        rpi_video_swap_buffer();
        rpi_video_clear();
    }
}
