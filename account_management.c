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