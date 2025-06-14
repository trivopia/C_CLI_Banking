#include "account_management.h"
#include "utils.h"
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
  char test[7];
} TEST;

void getTestString(TEST *pTest);

int main() {
  TEST *pTest = malloc(sizeof(TEST));
  if (pTest == NULL) {
    printf("ERROR");
    return -1;
  }
  printf("Please input 6 digit\n");
  getTestString(pTest);

  free(pTest);
  return 0;
}

int getString(char *str, int maxLength) {
  char buffer[1024];

  if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
    printf("Error Reading Input\n");
    exit(EXIT_FAILURE);
  }

  buffer[strcspn(buffer, "\n")] = '\0';

  int inputLength;
  inputLength = strlen(buffer);

  int lengthCompare;
  if (inputLength < maxLength - 1) {
    lengthCompare = -1;
    strncpy(str, buffer, maxLength);
  } else if (inputLength == maxLength - 1) {
    lengthCompare = 0;
    strncpy(str, buffer, maxLength);
  } else if (inputLength > maxLength - 1) {
    lengthCompare = 1;
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
      ;
  }

  return lengthCompare;
}

void getTestString(TEST *pTest) {

  while (true) {
    int flag = true;
    int lengthCompareState = getString(pTest->test, sizeof(pTest->test));

    if (lengthCompareState == -1) {
      printf("Please enter at least 6 digit\n");
      flag = false;
    } else if (lengthCompareState == 1) {
      printf("Input is too long!\n");
      flag = false;
    } else {
      printf("Success\n");
      printf("%s\n", pTest->test);
    }

    if (flag == true) {
      return;
    }
  }
}
