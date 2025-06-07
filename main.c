#include "account_management.h"
#include "utils.h"
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Function Prototypes
void stateUpdater(int *pState, int cases[], size_t numCases);
void registerNewAccount(int *pState);

int main() {
  int state = 99;
  int *pState = &state;

  int cases[] = {1, 2, 98};
  size_t numCases = sizeof(cases) / sizeof(cases[0]);

  while (state) {
    stateUpdater(pState, cases, numCases);
    switch (state) {
    case 1:
      registerNewAccount(pState);
      break;
    case 2:
      exit(EXIT_SUCCESS);
      break;
    default:
      break;
    }
  }
  return 0;
}

// Function for updating state
void stateUpdater(int *pState, int cases[], size_t numCases) {
  printf("\nChoose an action\n"
         "1. Register New Account\n"
         "2. Exit Program\n");

  while (true) {
    *pState = getIntInput();
    bool caseFound = false;

    for (size_t i = 0; i < numCases; i++) {
      if (*pState == cases[i]) {
        caseFound = true;
        break;
      }
    }

    if (!caseFound) {
      printf("Please enter a valid case\n");
    } else {
      break;
    }
  }
}

// Function for updating new account to the account_info.csv
void registerNewAccount(int *pState) {
  Account *newAccount;
  newAccount = malloc(sizeof(Account));

  srand(time(NULL));
  int newAccountNumber = rand() % 99999999;

  printf("registering account\n");

  // Getting string input for holderName
  printf("Enter your name\n");
  getHolderName(newAccount);

  // Getting string input for PIN (mimicking real world situation)
  printf("Enter your PIN(6-digit)\n");
  getPin(newAccount);

  // Getting char input for accountType
  printf("Enter the account type that you want to create\n"
         "S: Saving Accounts\n"
         "C: Checking Accounts\n");
  getAccountType(newAccount);

  // Randomizing accountNumber and setting initial balance to 0
  newAccount->accountNumber = newAccountNumber;
  newAccount->balance = 0.0;

  FILE *pFile = fopen("./dataBase/account_info.csv", "a");
  if (pFile == NULL) {
    perror("Could not open file\n");
    *pState = 0;
    return;
  }

  fprintf(pFile, "%d,%s,%s,%c\n", newAccount->accountNumber,
          newAccount->holderName, newAccount->pin, newAccount->accountType);

  fclose(pFile);
  free(newAccount);

  printf("Congratulation! You successfully created an account.\n"
         "Your initial balance is set to 0.00\n");
  *pState = 99;
  ;
}
