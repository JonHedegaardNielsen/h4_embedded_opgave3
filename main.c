#include "include/state_machine.h"

int main() {
  AppState appState = {.state = RECIEVE_ADRESS, .adress = 0, .index = -1};
  while (1) {
    state(&appState);
  }

  return 0;
}
