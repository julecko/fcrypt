#ifndef FILE_PROCESS_H
#define FILE_PROCESS_H

#include <stdbool.h>

void process_file(const char *filename, const char *password, bool encrypting);
void process_all_files(const char *pattern, const char *password, bool encrypting);

#endif