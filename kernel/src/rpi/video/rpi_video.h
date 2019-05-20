#ifndef _RPI_VIDEO_H_
#define _RPI_VIDEO_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../mailbox/rpi_mailbox.h"
#include "../mailbox/rpi_mailbox_interface.h"

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} rgb_t;

void rpi_video_init(int width, int height, int depth);
int rpi_video_width();
int rpi_video_height();
int rpi_video_depth();
int rpi_video_pitch();
void rpi_video_put_pixel(int x, int y, rgb_t color);
void rpi_video_swap_buffer();
void rpi_video_fill(rgb_t color);
void rpi_video_clear();


#endif
