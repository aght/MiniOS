#include <stdint.h>

#include "hal.h"
#include "../font/font.h"
#include "../uart/uart.h"

static uint32_t x_y_to_raw(uint32_t, uint32_t);
static uint32_t rgb_to_hex(rgb_t);

static uint32_t cursorX = 0;
static uint32_t cursorY = 0;

void hal_io_serial_init(void)
{
	uart0_init();
}

void hal_io_serial_puts(uint8_t *str)
{
	while (*str)
	{
		hal_io_serial_putc(*str++);
	}
}

void hal_io_serial_putc(uint8_t c)
{
	uart0_putc(c);
}

uint8_t hal_io_serial_getc()
{
	return uart0_getc();
}

void hal_io_video_puts(uint8_t *string, rgb_t color)
{
	while (*string)
	{
		hal_io_video_putc(*string++, color);
	}
}

void hal_io_video_putc(uint8_t c, rgb_t color)
{
	uint32_t y = cursorY;

	for (uint_fast32_t i = 0; i < 4; i++)
	{
		uint32_t bits = bit_font[(c * 4) + i];
		for (uint_fast32_t j = 0; j < 32; j++)
		{
			int xoffs = j % 8;

			if ((bits & 0x80000000) != 0)
			{
				hal_io_video_put_pixel(cursorX + xoffs, y, color);
			}

			bits <<= 1;
			if (xoffs == 7)
			{
				y++;
			}
		}
	}
	cursorX += FONT_WIDTH;
}

void hal_io_video_put_pixel(uint32_t x, uint32_t y, rgb_t color)
{
	hal_io_video_put_pixel_raw(x_y_to_raw(x, y), rgb_to_hex(color));
}

static uint32_t x_y_to_raw(uint32_t x, uint32_t y)
{
	return y * (3 * SCREEN_WIDTH) + 3 * x;
}

static uint32_t rgb_to_hex(rgb_t color)
{
	return ((color.b & 0xff) << 16) + ((color.g & 0xff) << 8) + (color.r & 0xff);
}
