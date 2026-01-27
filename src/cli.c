#include "cli_hide.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>


bool take_answer(const char *question) {
    char choice = 'n';
    printf("%s [y/N] ", question);
    
    choice = getchar();
    while (getchar() != '\n');
    
    return (choice == 'y' || choice == 'Y');
}

char *take_password(bool is_hidden) {
    char *password = malloc(256);
    if (!password) return NULL;

    if (is_hidden) hide_input();

    printf("Enter password: ");
    if (!fgets(password, 256, stdin)) {
        if (is_hidden) reveal_input();
        free(password);
        return NULL;
    }

    if (is_hidden) reveal_input();

    size_t len = strlen(password);
    if (len > 0 && password[len - 1] == '\n') {
        password[len - 1] = '\0';
    }

    printf("\n");
    return password;
}

void press_to_exit() {
    printf("Press any character to exit...");
    getchar();
}
