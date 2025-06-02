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

    int itemsRead = sscanf(buffer, "%d%c", &input, &charDetect);
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

void getStringInput(char *str, int maxLength) {
  while (true) {
    if (fgets(str, maxLength, stdin) == NULL) {
      printf("Error reading input\n");
      exit(EXIT_FAILURE);
    } else {
      str[strcspn(str, "\n")] = '\0';
      return;
    }
  }
}
