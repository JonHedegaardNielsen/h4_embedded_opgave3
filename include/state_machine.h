#include <stdint.h>
#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H
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
void state(AppState *app_state);
 
#endif
