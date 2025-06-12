#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Input validation related utilities
int getIntInput();
char getCharInput();
int getStringInput(char *str, int maxLength);

// File related utilities
int getLineCount(char filePath[]);

// Hashing related utilities
int generateRandomSalt(unsigned char *saltBuffer);
void binToHex(const unsigned char *bin, size_t binLength, char *hex);
void hexToBin(const char *hex, size_t hexLength, unsigned char *bin);
void hashPinWithSalt(const char *pinInput, const unsigned char *saltBin,
                     char *hashHexOutput);

#endif