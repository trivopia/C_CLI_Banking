#ifndef ACCOUNT_MANAGEMENT_H
#define ACCOUNT_MANAGEMENT_H

#include <openssl/rand.h>
#include <openssl/sha.h>
#include <stdio.h>
#include <string.h>

enum {
  ACCOUNT_NUMBER_LENGTH = 10,
  USER_ID_LENGTH = 10,
  PIN_LENGTH = 6,
  HASH_LENGTH_HEX = SHA256_DIGEST_LENGTH * 2,
  SALT_LENGTH_BYTES = 16,
  SALT_LENGTH_HEX = SALT_LENGTH_BYTES * 2,
  HOLDER_NAME_MAX = 50
};

typedef struct {
  char accountNumber[ACCOUNT_NUMBER_LENGTH + 1];
  char userID[USER_ID_LENGTH + 1];
  char pinHash[HASH_LENGTH_HEX + 1];
  char pinSalt[SALT_LENGTH_HEX + 1];
  long double balance;
  char holderName[HOLDER_NAME_MAX + 1];
  char accountType;
} Account;

void getHolderName(Account *pAccount);
void getUserID(Account *pAccount);
void getPin(char *pinInput, int bufferLength);
void hashPin(Account *pAccount);
void getAccountType(Account *pAccount);
int generateAccountNumber(Account *pAccount);
void loadAllAccountInfo(Account *accountBuffer, int totalAccounts);
int checkUserID(Account *logInBuffer, Account *existingAccounts,
                int totalAccounts, int *pAccountIndex);
int checkUserPIN(Account *existingAccounts, int *pAccountIndex,
                 Account *logInBuffer);
#endif
