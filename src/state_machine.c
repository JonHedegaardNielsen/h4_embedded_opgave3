#include <stdbool.h>
#include <stdint.h>
#include "../include/uart.h"
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

void state(AppState *app_state) {
  switch (app_state->state) {
  RECIEVE_ADRESS:
    break;
  RECIEVE_BIT_POSITION_IN_ADDRESS:
    break;
  RECIEVE_BIT_POSITION_VALUE:
    break;
  }
}

bool validate_bit(char charecter){
    return charecter == '0' || charecter == '1';
}
bool recieve_bit_position_value(const uint8_t *address) {
    uart_puts("inseat bit veardi fra 0-1\n");
    char c = uart_getchar();
    if (!validate_bit(c)){
        return false;
    }
    uint8_t value = c - '0';
    return true;
}

bool recieve_bit_position_in_address(const uint8_t address) {
    uart_puts("inseat position fra 0-7\n");
    char c = uart_getchar();
    uint8_t index = c - '0';
    return true;
}

bool recieve_adress(uint8_t *address) {
    uart_puts("inseat addresse fra 0-7\n");
    const uint8_t SIZE = 8;
    char buffer[SIZE];
    for (uint8_t i = 0; i < SIZE; i++)
    {
        buffer[i] = uart_getchar();
    }
    return true;
}
