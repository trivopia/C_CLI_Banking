#include "account_management.h"
#include "utils.h"
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

int generateAccountNumber(Account *newAccount) {
  int totalLines = getLineCount("./dataBase/account_info.csv");
  if (totalLines == -1) {
    printf("Could not open file\n");
    return -1;
  }

  int totalAccounts = totalLines - 1;
  if (totalAccounts == 0) {
    char firstAccount[10 + 1] = "0000000000";
    strcpy(newAccount->accountNumber, firstAccount);
    return 0;
  }

  char(*existingAccounts)[10 + 1] =
      malloc(totalAccounts * sizeof(char[10 + 1]));
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
  int loopGuard = 999999;
  int attempts = 0;

  while (true) {
    attempts++;
    if (attempts == loopGuard) {
      printf("Failed to generate a unique account number\n");
      free(existingAccounts);
      fclose(pFile);
      return -1;
    }

    bool flag = true;
    long long int randNum;
    char numberCandidate[10 + 1];

    randNum = rand() % 1000000000;
    sprintf(numberCandidate, "%010lld", randNum);

    for (size_t i = 0; i < totalAccounts; i++) {
      if (strcmp(numberCandidate, existingAccounts[i]) == 0) {
        flag = false;
        break;
      }
    }

    if (flag == true) {
      strcpy(newAccount->accountNumber, numberCandidate);
      break;
    }
  }

  free(existingAccounts);
  fclose(pFile);
  return 0;
}
