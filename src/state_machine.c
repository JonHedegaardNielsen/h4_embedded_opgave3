#include "../include/uart.h"
#include <stdbool.h>
#include <stdint.h>
typedef enum {
  RECIEVE_ADRESS,
  RECIEVE_BIT_POSITION_IN_ADDRESS,
  RECIEVE_BIT_POSITION_VALUE,
} State;

typedef struct {
  State state;
  uint8_t adress;
  uint8_t index;
} AppState;

bool validate_bit(char charecter) {
  return charecter == '0' || charecter == '1';
}

bool recieve_bit_position_value(uint8_t *address, const uint8_t position) {
  uart_puts("inseat bit veardi fra 0-1\n");
  char c = uart_getchar();
  if (!validate_bit(c)) {
    return false;
  }
  uint8_t bitvalue = c - '0';
  if (0 == bitvalue) {
    *address |= (1 << position);
  } else {
    *address &= ~(1 << position);
  }
  return true;
}

uint8_t recieve_bit_position_in_address(const uint8_t address) {
  uart_puts("inseat position fra 0-7\n");
  char c = uart_getchar();
  uint8_t index = c - '0';
  return index;
}

bool recieve_adress(uint8_t *address) {
  uart_puts("inseat addresse fra i hex\n");
  const uint8_t SIZE = 2;
  char buffer[SIZE];
  for (uint8_t i = 0; i < SIZE; i++) {
    char c = uart_getchar();
    if (c <= 'f' || c >= '0') {
      return false;
    }
    buffer[i] = c;
  }
  return true;
}

void state(AppState *app_state) {
  switch (app_state->state) {
  case RECIEVE_ADRESS:
    recieve_adress(&app_state->adress);
    break;
  case RECIEVE_BIT_POSITION_IN_ADDRESS:
    app_state->index = recieve_bit_position_in_address(app_state->adress);
    break;
  case RECIEVE_BIT_POSITION_VALUE:
    recieve_bit_position_value(&app_state->adress, app_state->index);
    break;
  }
}
