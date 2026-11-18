#include <avr/io.h>
#include <stdint.h>
#include "../include/ProjectDefines.h"

void uart_init(void) {
  UBRR0H = (uint8_t)(UBRR_VALUE >> 8);
  UBRR0L = (uint8_t)UBRR_VALUE;

  UCSR0B = (1 << TXEN0) | (1 << RXEN0);
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_putchar(char c) {
  while (!(UCSR0A & (1 << UDRE0)))
    ;
  UDR0 = c;
}

void uart_puts(const char *s) {
  while (*s)
    uart_putchar(*s++);
}

char uart_getchar(void) {
  while (!(UCSR0A & (1 << RXC0)))
    ;
  return UDR0;
}

void uart_putuint8(uint8_t value) {
  char buffer[3];
  uint8_t i = 0;
  if (value == 0) {
    uart_putchar('0');
    return;
  }
  while (value > 0) {
    buffer[i++] = (char)('0' + (value % 10));
    value /= 10;
  }
  while (i > 0) {
    uart_putchar(buffer[--i]);
  }
}

void uart_puthex16(uint16_t value) {
  static const char hex_digits[] = "0123456789ABCDEF";
  uart_puts("0x");
  for (int8_t i = 12; i >= 0; i -= 4) {
    uart_putchar(hex_digits[(value >> i) & 0x0F]);
  }
}

void uart_putbits8(uint8_t value) {
  for (uint8_t i = 0; i < 8; i++) {
    uart_putchar((value & (uint8_t)(0x80 >> i)) ? '1' : '0');
    if (i == 3) {
      uart_putchar(' ');
    }
  }
}
