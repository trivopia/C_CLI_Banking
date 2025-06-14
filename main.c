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
  clearScreen();
  srand(time(NULL));

  int state = 99;
  int *pState = &state;

  int cases[] = {1, 2, 98};
  size_t numCases = sizeof(cases) / sizeof(cases[0]);

  while (state) {
    stateUpdater(pState, cases, numCases);
    switch (state) {
    case 1:
      clearScreen();
      registerNewAccount(pState);
      break;
    case 2:
      clearScreen();
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
  printf("Choose an action\n"
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

// Function for registering new account
void registerNewAccount(int *pState) {
  Account *newAccount;
  newAccount = malloc(sizeof(Account));

  printf("registering account\n");

  // Getting string input for holderName
  printf("Enter your name\n");
  getHolderName(newAccount);
  clearScreen();

  // Getting user id
  printf("Enter USER ID(10-characters)\n");
  getUserID(newAccount);
  clearScreen();

  // Getting string input and hashing for pin
  printf("Enter your PIN(6-digit)\n");
  hashPin(newAccount);
  clearScreen();

  // Getting char input for accountType
  printf("Enter the account type that you want to create\n"
         "S: Saving Accounts\n"
         "C: Checking Accounts\n");
  getAccountType(newAccount);
  clearScreen();

  // Generating random unique 10-digit account number
  signed int errorCode = generateAccountNumber(newAccount);
  if (errorCode == -1) {
    printf("System Error\n");
    return;
  }

  // Setting initial balance to 0.0

  if (strcmp(newAccount->accountNumber, "0000000000") == 0) {
    newAccount->balance = 9999999999;
  } else {
    newAccount->balance = 0.0;
  }

  // Appending new account info to account_info.csv file
  FILE *pFile1 = fopen("./dataBase/account_info.csv", "a");
  if (pFile1 == NULL) {
    perror("Could not open file\n");
    *pState = 0;
    return;
  }

  FILE *pFile2 = fopen("./dataBase/account_balance.csv", "a");
  if (pFile2 == NULL) {
    perror("Could not open file\n");
    return;
  }

  fprintf(pFile1, "\n%s,%s,%s,%s,%s,%c", newAccount->accountNumber,
          newAccount->holderName, newAccount->userID, newAccount->pinHash,
          newAccount->pinSalt, newAccount->accountType);

  fprintf(pFile2, "\n%s,%.2Lf", newAccount->accountNumber, newAccount->balance);

  if (strcmp(newAccount->accountNumber, "0000000000") == 0) {
    printf("Congratulation! You successfully created a MASTER account.\n"
           "Your initial balance is set to 9999999999.00\n");
  } else {
    printf("Congratulation! You successfully created an account.\n"
           "Your initial balance is set to 0.00\n");
  }

  fclose(pFile1);
  fclose(pFile2);
  free(newAccount);
  *pState = 99;
  ;
}

// Function for log in
// Function for transfer
// Function for deleting account