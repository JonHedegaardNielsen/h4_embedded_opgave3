#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stdint.h>

typedef enum {
  RECEIVE_ADDRESS,
  RECEIVE_BIT_POSITION_IN_ADDRESS,
  RECEIVE_BIT_POSITION_VALUE,
} State;

typedef struct {
  State state;
  uint8_t address;
  uint8_t index;
  uint8_t bitvalue;
} AppState;

void state(AppState *app_state);

#endif
