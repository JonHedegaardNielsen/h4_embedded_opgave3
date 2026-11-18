#include "../include/state_machine.h"
#include "../include/uart.h"
#include <stdbool.h>
#include <stdint.h>

bool validate_hex_character(char character) {
  return (character >= '0' && character <= '9') ||
         (character >= 'a' && character <= 'f') ||
         (character >= 'A' && character <= 'F');
}

bool validate_bit(char character) {
  return character == '0' || character == '1';
}

char read_input_character(void) {
  char c = uart_getchar();
  if (c != '\r' && c != '\n') {
    uart_putchar(c);
  }
  return c;
}

uint8_t hex_character_to_digit(char character) {
  if (character >= 'a') {
    return character - 'a' + 10;
  }
  if (character >= 'A') {
    return character - 'A' + 10;
  }
  return character - '0';
}

void receive_address(uint8_t *address) {
  uart_puts("Tilstand: Modtag_Adresse - indtast adresse i hex, afslut med Enter eller ':'\r\n");
  uint8_t new_address = 0;
  char c;
  while (1) {
    c = read_input_character();
    if (c == '\r' || c == '\n' || c == ':') {
      break;
    }
    if (c == 'x' || c == 'X') {
      continue;
    }
    if (!validate_hex_character(c)) {
      continue;
    }
    new_address = (uint8_t)(new_address * 16 + hex_character_to_digit(c));
  }
  uart_puts("\r\n");
  *address = new_address;
}

bool receive_bit_position_in_address(uint8_t *index) {
  uart_puts("Tilstand: Modtag_Bit_Position_I_Adresse - indtast ciffer 0-7\r\n");
  char c;
  do {
    c = read_input_character();
  } while (c == '\r' || c == '\n');
  if (c < '0' || c > '7') {
    uart_puts("\r\nUgyldig position - indtast ny adresse\r\n");
    return false;
  }
  *index = c - '0';
  uart_puts("\r\n");
  return true;
}

bool receive_bit_position_value(uint8_t address, uint8_t position,
                                uint8_t *bitvalue) {
  uart_puts("Tilstand: Modtag_Bit_Position_Vaerdi - indtast 0 eller 1\r\n");
  char c;
  do {
    c = read_input_character();
  } while (c == '\r' || c == '\n');
  if (!validate_bit(c)) {
    uart_puts("\r\nUgyldig vaerdi - indtast ny adresse\r\n");
    return false;
  }
  uart_puts("\r\n");
  *bitvalue = c - '0';

  volatile uint8_t *p = (volatile uint8_t *)(uintptr_t)address;
  if (*bitvalue == 1) {
    *p |= (uint8_t)(1 << position);
  } else {
    *p &= (uint8_t)~(1 << position);
  }
  return true;
}

void state(AppState *app_state) {
  switch (app_state->state) {
  case RECEIVE_ADDRESS:
    receive_address(&app_state->address);
    app_state->state = RECEIVE_BIT_POSITION_IN_ADDRESS;
    break;
  case RECEIVE_BIT_POSITION_IN_ADDRESS:
    if (receive_bit_position_in_address(&app_state->index)) {
      app_state->state = RECEIVE_BIT_POSITION_VALUE;
    } else {
      app_state->state = RECEIVE_ADDRESS;
    }
    break;
  case RECEIVE_BIT_POSITION_VALUE:
    if (receive_bit_position_value(app_state->address, app_state->index,
                                   &app_state->bitvalue)) {
      uart_puts("Adresse: ");
      uart_puthex16(app_state->address);
      uart_puts("\r\nBit position: ");
      uart_putuint8(app_state->index);
      uart_puts("\r\nBit vaerdi: ");
      uart_putuint8(app_state->bitvalue);
      uart_puts("\r\nBit moenster: ");
      uart_putbits8(*(uint8_t *)(uintptr_t)app_state->address);
      uart_puts("\r\n");
    }
    app_state->state = RECEIVE_ADDRESS;
    break;
  }
}
