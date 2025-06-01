#include "utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getIntInput() {
  char buffer[1024];
  int input;

  while (true) {
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
      printf("Error reading input\n");
      exit(EXIT_FAILURE);
    }

    if (sscanf(buffer, "%d", &input) == true) {
      return input;
    } else {
      printf("invalid input. Please enter a number: ");
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

    if (sscanf(buffer, "%c", &input)) {
      return input;
    } else {
      printf("Invalid input. Enter a character");
    }
  }
}