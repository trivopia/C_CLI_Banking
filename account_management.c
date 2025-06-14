#include "account_management.h"
#include "utils.h"
#include <ctype.h>
#include <math.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void getHolderName(Account *pAccount) {
  while (true) {
    bool flag = true;

    int returnResult =
        getStringInput(pAccount->holderName, sizeof(pAccount->holderName));

    if (pAccount->holderName[0] == '\0') {
      printf("Invalid input. Please enter your name\n");
      flag = false;
    } else if (returnResult == 1) {
      printf("Input too long! Please enter your name\n");
      flag = false;
    } else {
      for (int i = 0; pAccount->holderName[i] != '\0'; i++) {
        if (!isalpha(pAccount->holderName[i]) &&
            !isspace(pAccount->holderName[i])) {
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

  for (int i = 0; pAccount->holderName[i] != '\0'; i++) {
    pAccount->holderName[i] = toupper(pAccount->holderName[i]);
  }
}

void getPin(char *pinInput, int bufferLength) {
  while (true) {
    bool flag = true;

    int returnResult = getStringInput(pinInput, bufferLength);

    if (returnResult == -1) {
      printf("Please enter 6 digits\n");
      flag = false;
    } else if (returnResult == 1) {
      printf("Please only enter 6 digits\n");
      flag = false;
    } else {
      for (size_t i = 0; i < PIN_LENGTH; i++) {
        if (!isdigit(pinInput[i])) {
          printf("Please only enter numbers\n");
          flag = false;
          break;
        }
      }
    }

    if (flag == true) {
      return;
    }
  }
}

void hashPin(Account *pAccount) {
  char pinInput[PIN_LENGTH + 1];
  getPin(pinInput, sizeof(pinInput));

  unsigned char saltBuffer[SALT_LENGTH_BYTES];
  generateRandomSalt(saltBuffer);

  binToHex(saltBuffer, SALT_LENGTH_BYTES, pAccount->pinSalt);

  hashPinInputWithSalt(pinInput, saltBuffer, pAccount->pinHash);
}

void getAccountType(Account *pAccount) {
  while (true) {
    pAccount->accountType = toupper(getCharInput());
    if (pAccount->accountType == 'S' || pAccount->accountType == 'C') {
      break;
    } else {
      printf("Invalid choice. Please select a valid account type\n");
    }
  }
}

int generateAccountNumber(Account *pAccount) {
  int totalLines = getLineCount("./dataBase/account_info.csv");
  if (totalLines == -1) {
    printf("Could not open file\n");
    return -1;
  }

  // For the master account
  int totalAccounts = totalLines - 1;
  if (totalAccounts == 0) {
    char firstAccount[ACCOUNT_NUMBER_LENGTH + 1] = "0000000000";
    strcpy(pAccount->accountNumber, firstAccount);
    return 0;
  }

  // Creating pointers to string array memory blocks
  char(*existingAccounts)[ACCOUNT_NUMBER_LENGTH + 1] =
      malloc(totalAccounts * sizeof(char[ACCOUNT_NUMBER_LENGTH + 1]));
  if (existingAccounts == NULL) {
    perror("Unable to allocate memory for existing accounts");
    return -1;
  }

  FILE *pFile = fopen("./dataBase/account_info.csv", "r");
  if (pFile == NULL) {
    perror("File could not be opened");
    free(existingAccounts);
    return -1;
  }

  char buffer[1024];

  // Call to skip the header
  fgets(buffer, sizeof(buffer), pFile);

  for (size_t i = 0; i < totalAccounts; i++) {
    if (fgets(buffer, sizeof(buffer), pFile) == NULL) {
      printf("Failed reading file content from line %ld\n", i + 1);
      free(existingAccounts);
      fclose(pFile);
      return -1;
    }

    buffer[strcspn(buffer, "\n")] = '\0';

    // tokenizing and copying the first element from each line
    char *token = strtok(buffer, ",");
    if (token != NULL) {
      strcpy(existingAccounts[i], token);
    } else {
      printf("Failed to tokenize line %ld\n", i + 1);
      free(existingAccounts);
      fclose(pFile);
      return -1;
    }
  }

  // Generate new unique 8-digit account number
  // Attempts are limited due to performance issue
  int loopGuard = 999999;
  int attempts = 0;

  while (true) {
    bool flag = true;

    attempts++;
    if (attempts == loopGuard) {
      printf("Failed to generate a unique account number\n");
      free(existingAccounts);
      fclose(pFile);
      return -1;
    }

    // + '0' to make the digits into ASCII chars
    char numberCandidate[ACCOUNT_NUMBER_LENGTH + 1];
    for (size_t i = 0; i < ACCOUNT_NUMBER_LENGTH; i++) {
      numberCandidate[i] = '0' + rand() % 10;
    }
    numberCandidate[10] = '\0';

    for (size_t i = 0; i < totalAccounts; i++) {
      if (strcmp(numberCandidate, existingAccounts[i]) == 0) {
        flag = false;
        break;
      }
    }

    if (flag == true) {
      strcpy(pAccount->accountNumber, numberCandidate);
      break;
    }
  }

  free(existingAccounts);
  fclose(pFile);
  return 0;
}
