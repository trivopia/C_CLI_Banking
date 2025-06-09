#include "utils.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getIntInput() {
  char buffer[1024];
  int input;
  char charDetect;

  while (true) {
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
      printf("Error reading input\n");
      exit(EXIT_FAILURE);
    }

    buffer[strcspn(buffer, "\n")] = '\0';

    int itemsRead = sscanf(buffer, "%d %c", &input, &charDetect);
    if (itemsRead == 0) {
      printf("Invalid input. Please enter a number: ");
    } else if (itemsRead == 1) {
      return input;
    } else {
      printf("Invalid input. Please enter only numbers: ");
    }
  }
}

char getCharInput() {
  char buffer[1024];
  char input;

  while (true) {
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
      printf("Error reading input\n");
      exit(EXIT_FAILURE);
    }

    if (sscanf(buffer, "%c", &input) == true) {
      if (isalpha(input)) {
        return input;
      } else {
        printf("Invalid input. Enter a character: ");
      }
    } else {
      printf("Invalid input. Enter a character: ");
    }
  }
}

int getStringInput(char *str, int maxLength) {
  int c;
  bool charOverflow = false;

  if (fgets(str, maxLength, stdin) == NULL) {
    printf("Error Reading Input\n");
    return -1;
  }

  if (str[strlen(str) - 1] != '\n') {
    charOverflow = true;
    while (((c = getchar()) != '\n' && c != EOF))
      ;
  }

  str[strcspn(str, "\n")] = '\0';
  return charOverflow ? 1 : 0;
}

int getLineCount(char fileName[]) {
  long fileSize;
  int lineCounts = 0;
  int c;

  FILE *pFile = fopen(fileName, "r");
  if (pFile == NULL) {
    perror("Could not open file\n");
    return -1;
  }

  fseek(pFile, 0, SEEK_END);
  fileSize = ftell(pFile);

  if (fileSize == 0) {
    printf("File is empty");
    fclose(pFile);
    return 0;
  }

  fseek(pFile, 0, SEEK_SET);
  while ((c = fgetc(pFile)) != EOF) {
    if (c == '\n') {
      lineCounts++;
    }
  }

  if (fileSize > 0 && c != '\n') {
    lineCounts++;
  }

  fclose(pFile);

  return lineCounts;
}
