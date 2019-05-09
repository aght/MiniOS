#ifndef _UART_
#define _UART_

#include "uart.h"

static void str(uint32_t, uint32_t);
static uint32_t ldr(uint32_t);
static void delay(uint32_t);

void uart0_init(void)
{
    // Disable UART0.
    str(UART0_CR, 0x00000000);

    // Setup the GPIO pin 14 && 15.

    // Disable pull up/down for all GPIO pins & delay for 150 cycles.
    str(GPPUD, 0x00000000);
    delay(150);
    // Disable pull up/down for pin 14,15 & delay for 150 cycles.
    str(GPPUDCLK0, (1 << 14) | (1 << 15));
    delay(150);
    // Write 0 to GPPUDCLK0 to make it take effect.
    str(GPPUDCLK0, 0x00000000);
    // Clear pending interrupts.
    str(UART0_ICR, 0x7FF);
    // Set integer & fractional part of baud rate.
    // Divider = UART_CLOCK/(16 * Baud)
    // Fraction part register = (Fractional part * 64) + 0.5
    // UART_CLOCK = 3000000; Baud = 115200.

    // Divider = 3000000 / (16 * 115200) = 1.627 = ~1.
    str(UART0_IBRD, 1);
    // Fractional part register = (.627 * 64) + 0.5 = 40.6 = ~40.
    str(UART0_FBRD, 40);
    // Enable FIFO & 8 bit data transmissio (1 stop bit, no parity).
    str(UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6));
    // Mask all interrupts.
    str(UART0_IMSC, (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) |
                        (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));
    // Enable UART0, receive & transfer part of UART.
    str(UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));
}

void uart0_putc(uint8_t c)
{
    //wait for it to be ready
    while (ldr(UART0_FR) & (1 << 5))
        ;

    //write
    str(UART0_DR, c);
}

uint8_t uart0_getc(void)
{
    //wait for it to be ready
    while (ldr(UART0_FR) & (1 << 4))
        ;

    //write
    return ldr(UART0_DR);
}

static void str(uint32_t address, uint32_t v)
{
    *(volatile uint32_t *)address = v;
}

static uint32_t ldr(uint32_t address)
{
    return *(volatile uint32_t *)address;
}

static void delay(uint32_t count)
{
    asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
                 : "=r"(count)
                 : [count] "0"(count)
                 : "cc");
}

#endif
