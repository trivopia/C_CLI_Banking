#include "account_management.h"
#include "utils.h"
#include <stdio.h>

int main() {
  printf("Input pin: ");
  char pinInput[PIN_LENGTH + 2];
  getPin(pinInput, sizeof(pinInput));
  return 0;
}