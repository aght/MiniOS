#ifndef _RPI_VIDEO_
#define _RPI_VIDEO_

#include "rpi_video.h"

#include "../../drivers/stdio/emb-stdio.h"

static volatile uint8_t *frame_buffer = NULL;
static volatile int width = 0;
static volatile int height = 0;
static volatile int depth = 0;
static volatile int pitch = 0;

static volatile int virtual_offset = 0;

void rpi_video_init(int w, int h, int d) {
    rpi_property_init();
    rpi_property_add_tag(TAG_ALLOCATE_BUFFER);
    rpi_property_add_tag(TAG_SET_PHYSICAL_SIZE, w, h);
    rpi_property_add_tag(TAG_SET_VIRTUAL_SIZE, w, h * 2);
    rpi_property_add_tag(TAG_SET_DEPTH, d);
    rpi_property_add_tag(TAG_GET_VIRTUAL_OFFSET);
    rpi_property_add_tag(TAG_GET_PITCH);
    rpi_property_add_tag(TAG_GET_PHYSICAL_SIZE);
    rpi_property_add_tag(TAG_GET_DEPTH);
    rpi_property_process();

    rpi_mailbox_property_t *mp;

    if ((mp = rpi_property_get(TAG_GET_PHYSICAL_SIZE))) {
        width = mp->data.buffer_32[0];
        height = mp->data.buffer_32[1];
        virtual_offset = mp->data.buffer_32[1];
    }

    if ((mp = rpi_property_get(TAG_GET_DEPTH))) {
        depth = mp->data.buffer_32[0];
    }

    if ((mp = rpi_property_get(TAG_GET_PITCH))) {
        pitch = mp->data.buffer_32[0];
    }

    if ((mp = rpi_property_get(TAG_ALLOCATE_BUFFER))) {
        frame_buffer = (uint8_t *)mp->data.buffer_32[0];
    }
}

int rpi_video_width() {
    return width;
}

int rpi_video_height() {
    return height;
}

int rpi_video_depth() {
    return depth;
}

int rpi_video_pitch() {
    return pitch;
}

void rpi_video_put_pixel(int x, int y, rgb_t color) {
    int offset = (x * (depth >> 3)) + (y * pitch);

    if (depth == 32) {
        // Cannot support opacity
        // frame_buffer[offset++] = color.r;
        // frame_buffer[offset++] = color.g;
        // frame_buffer[offset++] = color.b;
        // frame_buffer[offset++] = color.a;
    } else if (depth == 24) {
        frame_buffer[offset++] = color.r;
        frame_buffer[offset++] = color.g;
        frame_buffer[offset++] = color.b;
    } else if (depth == 16) {
        *(unsigned short *)&frame_buffer[offset] = ((color.r >> 3) << 11) | ((color.g >> 2) << 5) | (color.b >> 3);
    } else {
        // Do nothing
    }
}

void rpi_video_swap_buffer() {
    memcpy(frame_buffer, &frame_buffer[(0 * (depth >> 3)) + (virtual_offset * pitch)], (0 * (depth >> 3)) + (virtual_offset * pitch) * sizeof(uint8_t));
}

void rpi_video_rect(int x, int y, int w, int h, rgb_t color) {
    const uint_fast32_t ymax = y + h + virtual_offset;
    const uint_fast32_t xmax = x + w;
    
    for (uint_fast32_t _y = y + virtual_offset; _y < ymax; _y++) {
        for (uint_fast32_t _x = x; _x < xmax; _x++) {
            rpi_video_put_pixel(_x, _y, color);
        }
    }
}

void rpi_video_fill(rgb_t color) {
    rpi_video_rect(0, 0, width, height, color);
}

void rpi_video_clear() {
    memset(&frame_buffer[(0 * (depth >> 3)) + (virtual_offset * pitch)], 0, (0 * (depth >> 3)) + (virtual_offset * pitch) * sizeof(uint8_t));
}

void rpi_video_clearf() {
    memset(frame_buffer, 0, (width * (depth >> 3)) + (height * pitch) * sizeof(uint8_t));
}

#endif
