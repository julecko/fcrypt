#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>

char *make_enc_filename(const char *filename);
char *make_dec_filename(const char *filename);
bool file_exist(const char *filename);
void remove_file(const char *filename);

#endif