#include "utils.h"
#include "account_management.h"
#include <ctype.h>
#include <openssl/rand.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Input validation related utilities
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

// File related utilities
int getLineCount(char filePath[]) {
  long fileSize;
  int lineCounts = 0;
  int c;

  FILE *pFile = fopen(filePath, "r");
  if (pFile == NULL) {
    perror("Could not open file\n");
    return -1;
  }

  fseek(pFile, 0, SEEK_END);
  fileSize = ftell(pFile);

  if (fileSize == 0) {
    printf("File is empty\n");
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

// Hashing related utilities
int generateRandomSalt(unsigned char *saltBuffer) {
  if (RAND_bytes(saltBuffer, SALT_LENGTH_BYTES) != 1) {
    printf("Error: Failed to generate random salt");
    return -1;
  }
  return 0;
}

void binToHex(const unsigned char *bin, size_t binLength, char *hex) {
  for (size_t i = 0; i < binLength; i++) {
    sprintf(&hex[i * 2], "%02x", bin[i]);
  }
  hex[binLength * 2] = '\0';
}

void hexToBin(const char *hex, size_t hexLength, unsigned char *bin) {
  for (size_t i = 0; i < hexLength / 2; i++) {
    sscanf(hex + (i * 2), "%02hhx", &bin[i]);
  }
}

void hashPinWithSalt(const char *pinInput, const unsigned char *saltBin,
                     char *hashHexOutput) {
  size_t totalLength = PIN_LENGTH + SALT_LENGTH_BYTES;
  unsigned char *dataToHash = malloc(totalLength);
  if (dataToHash == NULL) {
    perror("Error: could not allocate memory for hashing\n");
    exit(EXIT_FAILURE);
  }
  memcpy(dataToHash, pinInput, PIN_LENGTH);
  memcpy(dataToHash + PIN_LENGTH, saltBin, SALT_LENGTH_BYTES);

  unsigned char hashBin[SHA256_DIGEST_LENGTH];

  SHA256(dataToHash, totalLength, hashBin);

  binToHex(hashBin, SHA224_DIGEST_LENGTH, hashHexOutput);

  free(dataToHash);
}
