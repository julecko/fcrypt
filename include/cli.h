#ifndef CLI_H
#define CLI_H

#include <stdbool.h>

char *take_password(bool is_hidden);
void press_to_exit();
bool take_answer(const char *question);

#endif