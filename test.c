#include "account_management.h"
#include "utils.h"
#include <stdio.h>

int main() {
  Account *newAccount = malloc(sizeof(Account));
  printf("Input pin: ");
  char pinInput[PIN_LENGTH + 2];
  getPin(pinInput, sizeof(pinInput));

  unsigned char saltBuffer[SALT_LENGTH_BYTES];
  generateRandomSalt(saltBuffer);
  binToHex(saltBuffer, SALT_LENGTH_BYTES, newAccount->pinSalt);


  hashPinInputWithSalt(pinInput, saltBuffer, newAccount->pinHash);
  
  free(newAccount);
  return 0;
}
