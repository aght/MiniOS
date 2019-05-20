#ifndef HAL_H_
#define HAL_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../rpi/video/rpi_video.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define FONT_HEIGHT 16
#define FONT_WIDTH 8

uint32_t hal_io_get_cursor_x();
uint32_t hal_io_get_cursor_y();
void hal_io_set_cursor_x(uint32_t);
void hal_io_set_cursor_y(uint32_t);

uint32_t hal_io_video_init(void);
void hal_io_video_puts(uint8_t *);
void hal_io_video_putc(uint8_t);
void hal_io_video_put_pixel(uint_fast32_t, uint_fast32_t, rgb_t);

void hal_io_video_set_brush_color(rgb_t);
void hal_io_video_set_fill_color(rgb_t);

void hal_io_serial_init(void);
void hal_io_serial_puts(uint8_t *);
void hal_io_serial_putc(uint8_t);
uint8_t hal_io_serial_getc();

void hal_io_video_put_pixel_raw(uint32_t, uint32_t);

void hal_io_video_clear();

#endif
