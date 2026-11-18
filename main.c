#include "include/state_machine.h"
#include "include/uart.h"

int main() {
  uart_init();

  AppState appState = {
      .state = RECEIVE_ADDRESS, .address = 0, .index = 0, .bitvalue = 0};
  while (1) {
    state(&appState);
  }

  return 0;
}
