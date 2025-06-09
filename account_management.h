#ifndef ACCOUNT_MANAGEMENT_H
#define ACCOUNT_MANAGEMENT_H

#include <stdio.h>
#include <string.h>

typedef struct {
  char accountNumber[10 + 1];
  char pin[6 + 2];
  long double balance;
  char holderName[100 + 2];
  char accountType;

} Account;

void getHolderName(Account *newAccount);
void getPin(Account *newAccount);
void getAccountType(Account *newAccount);
int generateAccountNumber(Account *newAccount);

#endif
