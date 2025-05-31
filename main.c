#include <stdio.h>

int main() {
  FILE *pFile = fopen("./dataBase/account_info.csv", "r");
  if (pFile == NULL) {
    perror("File could not be opened");
    return 1;
  }

  char buffer[1024];
  while (fgets(buffer, sizeof(buffer), pFile) != 0) {
    printf("%s\n", buffer);
  }

  fclose(pFile);
  return 0;
}