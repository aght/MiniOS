#ifndef HAL_H
#define HAL_H

#include <stdbool.h>
#include <stdint.h>

typedef struct
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
} rgb_t;

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define FONT_HEIGHT 16
#define FONT_WIDTH 8

uint32_t hal_io_video_init(void);
void hal_io_video_puts(uint8_t *);
void hal_io_video_putc(uint8_t);
void hal_io_video_put_pixel(uint32_t, uint32_t, rgb_t);

void hal_io_serial_init(void);
void hal_io_serial_puts(uint8_t *);
void hal_io_serial_putc(uint8_t);
uint8_t hal_io_serial_getc();

void hal_io_video_put_pixel_raw(uint32_t, uint32_t);

#endif
