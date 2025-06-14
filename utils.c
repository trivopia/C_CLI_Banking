#include "utils.h"
#include "account_management.h"
#include <ctype.h>
#include <openssl/rand.h>
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

    // why not use isdigit()? because it is too complicated. i would have to
    // parse around each element
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

    if (strchr(buffer, '\n') == NULL) {
      int c;
      while ((c = getchar()) != '\n' && c != EOF)
        ;
      printf("Input too long. Enter a single character]\n");
      continue;
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
  char buffer[1024];

  if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
    printf("Error Reading Input\n");
    exit(EXIT_FAILURE);
  }

  if (strchr(buffer, '\n') == NULL) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
      ;
    return 1;
  }

  buffer[strcspn(buffer, "\n")] = '\0';
  int inputLength = strlen(buffer);

  if (inputLength < maxLength - 1) {
    strncpy(str, buffer, maxLength);
    return -1;
  } else if (inputLength == maxLength - 1) {
    strncpy(str, buffer, maxLength);
    return 0;
  } else {
    return 1;
  }
}

// param 0 is a binary of salt
int generateRandomSalt(unsigned char *saltBuffer) {
  if (RAND_bytes(saltBuffer, SALT_LENGTH_BYTES) != 1) {
    printf("Error: Failed to generate random salt");
    return -1;
  }
  return 0;
}

void binToHex(const unsigned char *bin, size_t binLength, char *hex) {
  for (size_t i = 0; i < binLength; i++) {
    sprintf(
        hex + (i * 2), "%02x",
        bin[i]); // hex + (i*2) meaning 2 hex for each byte and zero for padding
  }
  hex[binLength * 2] = '\0';
}

void hexToBin(const char *hex, size_t hexLength, unsigned char *bin) {
  for (size_t i = 0; i < hexLength / 2;
       i++) { // i < hexLength / 2 because 2 hex = 1 byte
    sscanf(hex + (i * 2), "%02hhx", bin + i);
  }
}

void hashPinInputWithSalt(const char *pinInput, const unsigned char *saltBin,
                          char *hashHexOutput) {
  size_t totalLength = PIN_LENGTH + SALT_LENGTH_BYTES;

  unsigned char *dataToHash = malloc(totalLength);
  if (dataToHash == NULL) {
    perror("Error: could not allocate memory for hashing\n");
    exit(EXIT_FAILURE);
  }

  // using memcpy to copy raw memory for any data type. And it does'nt stop at
  // Null terminator
  memcpy(dataToHash, pinInput, PIN_LENGTH);
  memcpy(dataToHash + PIN_LENGTH, saltBin, SALT_LENGTH_BYTES);

  unsigned char hashBin[SHA256_DIGEST_LENGTH];

  SHA256(dataToHash, totalLength, hashBin);

  binToHex(hashBin, SHA256_DIGEST_LENGTH, hashHexOutput);

  free(dataToHash);
}

int getLineCount(char filePath[]) {
  long fileSize;
  int lineCounts = 0;
  int c;

  FILE *pFile = fopen(filePath, "r");
  if (pFile == NULL) {
    perror("Could not open file\n");
    return -1;
  }

  // to check if file is empty
  fseek(pFile, 0, SEEK_END);
  fileSize = ftell(pFile);

  if (fileSize == 0) {
    printf("File is empty\n");
    fclose(pFile);
    return 0;
  }

  // the line counting logic
  fseek(pFile, 0, SEEK_SET);
  while ((c = fgetc(pFile)) != EOF) {
    if (c == '\n') {
      lineCounts++;
    }
  }

  // for the last line
  if (fileSize > 0 && c != '\n') {
    lineCounts++;
  }

  fclose(pFile);

  return lineCounts;
}

void clearScreen() {
  printf("\033[2J\033[H"); // a way for clearing UNIX system CLI
  fflush(stdout);
}
