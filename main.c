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
void logIn(int *pState);
int userActionTab();

int main() {
  clearScreen();
  srand(time(NULL));

  int state;
  int *pState = &state;

  int cases[] = {1, 2, 3};
  size_t numCases = sizeof(cases) / sizeof(cases[0]);

  stateUpdater(pState, cases, numCases);

  while (state) {
    switch (state) {
    case 1:
      clearScreen();
      registerNewAccount(pState);
      break;
    case 2:
      clearScreen();
      logIn(pState);
      userActionTab();
      exit(EXIT_SUCCESS);
      break;
    case 3:
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
         "2. Log In\n"
         "3. Exit Program\n");

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
    *pState = 99;
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
}

// Function for log in
void logIn(int *pState) {

  // Get all file info
  FILE *pFile = fopen("./dataBase/account_info.csv", "r");
  if (pFile == NULL) {
    printf("Error opening file\n");
    exit(EXIT_FAILURE);
  }

  int totalAccounts = getLineCount("./dataBase/account_info.csv") - 1;
  Account *existingAccounts = malloc(sizeof(Account) * totalAccounts);

  char buffer[1024];

  // skip header
  fgets(buffer, sizeof(buffer), pFile);
  buffer[strcspn(buffer, "\n")] = '\0';

  for (size_t i = 0; i < totalAccounts; i++) {
    fgets(buffer, sizeof(buffer), pFile);
    buffer[strcspn(buffer, "\n")] = '\0';

    char *token = strtok(buffer, ",");
    strcpy((existingAccounts + i)->accountNumber, token);

    token = strtok(NULL, ",");
    strcpy((existingAccounts + i)->holderName, token);

    token = strtok(NULL, ",");
    strcpy((existingAccounts + i)->userID, token);

    token = strtok(NULL, ",");
    strcpy((existingAccounts + i)->pinHash, token);

    token = strtok(NULL, ",");
    strcpy((existingAccounts + i)->pinSalt, token);

    token = strtok(NULL, ",");
    (existingAccounts + i)->accountType = token[0];
  }

  // Check ID
  Account *logInBuffer = malloc(sizeof(Account));
  int accountIndex = -1;
  bool idMatch = false;

  while (true) {
    printf("Input your USER ID: ");
    getUserID(logInBuffer);

    for (size_t i = 0; i < totalAccounts; i++) {
      if (strcmp(logInBuffer->userID, (existingAccounts + i)->userID) == 0) {
        accountIndex = i;
        idMatch = true;
        break;
      }
    }

    if (idMatch) {
      clearScreen();
      printf("USER ID: %s\n", logInBuffer->userID);
      break;
    } else {
      clearScreen();
      printf("ID not found\n");
    }
  }

  // Check pin
  while (true) {
    char pinInput[PIN_LENGTH + 1];
    printf("Input your PIN: ");
    getPin(pinInput, sizeof(pinInput));

    unsigned char saltBuffer[SALT_LENGTH_BYTES];
    hexToBin((existingAccounts + accountIndex)->pinSalt, SALT_LENGTH_HEX,
             saltBuffer);

    hashPinInputWithSalt(pinInput, saltBuffer, logInBuffer->pinHash);

    if (strcmp(logInBuffer->pinHash,
               (existingAccounts + accountIndex)->pinHash) == 0) {
      clearScreen();
      printf("Welcome %s!\n", (existingAccounts + accountIndex)->holderName);
      break;
    } else {
      clearScreen();
      printf("Wrong PIN\n");
      printf("USER ID: %s\n", logInBuffer->userID);
    }
  }

  free(logInBuffer);
  free(existingAccounts);
  fclose(pFile);

  *pState = 99;
}

int userActionTab() {
  printf("Choose an action\n"
         "1. Deposit\n"
         "2. Transfer\n"
         "3. Exit Program\n");
  return 1;
}

// Function for transfer
// Function for deleting account