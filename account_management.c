#include "account_management.h"
#include "utils.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void getHolderName(Account *newAccount) {
  while (true) {
    bool flag = true;

    getStringInput(newAccount->holderName, sizeof(newAccount->holderName));

    if (newAccount->holderName[0] == '\0') {
      printf("Invalid input. Please enter your name\n");
      flag = false;
    } else {
      for (int i = 0; newAccount->holderName[i] != '\0'; i++) {
        if (!isalpha(newAccount->holderName[i]) &&
            !isspace(newAccount->holderName[i])) {
          printf("Invalid input. Please enter your name\n");
          flag = false;
          break;
        }
      }
    }

    if (flag == true) {
      break;
    }
  }

  for (int i = 0; newAccount->holderName[i] != '\0'; i++) {
    newAccount->holderName[i] = toupper(newAccount->holderName[i]);
  }
}

void getPin(Account *newAccount) {
  while (true) {
    int overFlow;
    bool flag = true;

    overFlow = getStringInput(newAccount->pin, sizeof(newAccount->pin));

    int pinLength = strlen(newAccount->pin);

    if (pinLength < 6) {
      printf("Please enter 6 digits\n");
      flag = false;
    } else if (overFlow) {
      printf("Please only enter 6 digits\n");
      flag = false;
    } else {
      for (size_t i = 0; i < pinLength; i++) {
        if (!isdigit(newAccount->pin[i])) {
          printf("Please only enter numbers\n");
          flag = false;
          break;
        }
      }
    }

    if (flag == true) {
      break;
    }
  }
}

void getAccountType(Account *newAccount) {
  while (true) {
    newAccount->accountType = toupper(getCharInput());
    if (newAccount->accountType == 'S' || newAccount->accountType == 'C') {
      break;
    } else {
      printf("Invalid choice. Please select a valid account type\n");
    }
  }
}