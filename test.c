#include "account_management.h"
#include "utils.h"
#include <stdio.h>

int main() {
  int counts;
  counts = getLineCount("./dataBase/account_info.csv");
  printf("%d\n", counts);
  return 0;
}