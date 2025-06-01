#include "utils.h"
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
  int accountNumber;
  int pin;
  long double balance;
  char holderName[100];
  char accountType;

} Account;

// Function Prototypes
void stateUpdater(int *pState);
void registerNewAccount(int *pState);

int main() {
  int state = 99;
  int *pState = &state;

  while (state) {
    stateUpdater(pState);
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
void stateUpdater(int *pState) {
  char buffer[1024];
  int choice = 0;

  printf("\nChoose an action\n"
         "1. Register New Account\n"
         "2. Exit Program\n");

  if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
    perror("Error in reading input");
    *pState = 0;
    return;
  }

  if (sscanf(buffer, "%d", &choice) == 1) {
    *pState = choice;
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

  // Getting int input for PIN
  printf("Enter your PIN(6-digit)\n");
  newAccount->pin = getIntInput();

  // Getting char input for accountType
  printf("Enter the account type that you want to create\n"
         "S: Saving Accounts\n"
         "C: Checking Accounts\n");
  while (true) {
    newAccount->accountType = toupper(getCharInput());
    if (newAccount->accountType == 'S' || newAccount->accountType == 'C') {
      break;
    } else {
      printf("Invalid choice. Please select a valid account type\n");
    }
  }

  // Randomizing accountNumber and setting initial balance to 0
  newAccount->accountNumber = newAccountNumber;
  newAccount->balance = 0.0;

  FILE *pFile = fopen("./dataBase/account_info.csv", "a");
  if (pFile == NULL) {
    perror("Could not open file\n");
    *pState = 0;
    return;
  }

  fprintf(pFile, "\n%d,%s,%d,%c", newAccount->accountNumber,
          newAccount->holderName, newAccount->pin, newAccount->accountType);

  fclose(pFile);
  free(newAccount);

  printf("Congratulation! You successfully created an account.\n"
         "Your initial balance is set to 0.00\n");
  *pState = 99;
  ;
}
