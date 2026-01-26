#ifndef CRYPT_CRYPT_H
#define CRYPT_CRYPT_H

int encrypt(const char *input_filename, const char *password);
int decrypt(const char *input_filename, const char *password);

#endif