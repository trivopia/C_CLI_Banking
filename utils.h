#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getIntInput();
char getCharInput();
int getStringInput(char *str, int maxLength);
int getLineCount(char filePath[]);

#endif