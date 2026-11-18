#ifndef UART_H
#define UART_H

#include <stdint.h>

void uart_init(void);
void uart_putchar(char c);
void uart_puts(const char *s);
char uart_getchar(void);
void uart_putuint8(uint8_t value);
void uart_puthex16(uint16_t value);
void uart_putbits8(uint8_t value);

#endif
