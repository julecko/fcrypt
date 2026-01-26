#include <stdio.h>
#include <windows.h>
#include <stdbool.h>


bool take_answer(const char *question) {
    char choice = 'n';
    printf("%s [y/N] ", question);
    
    choice = getchar();
    while (getchar() != '\n');

    if (choice == 'y' || choice == 'Y') {
        return true;
    } else {
        return false;
    }
}

char *take_password(bool is_hidden) {
    char *password = malloc(256);
    if (!password) return NULL;

    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);

    if (is_hidden) {
        SetConsoleMode(hStdin, mode & ~(ENABLE_ECHO_INPUT));
    }

    printf("Enter password: ");
    if (!fgets(password, 256, stdin)) {
        free(password);
        SetConsoleMode(hStdin, mode);
        return NULL;
    }

    size_t len = strlen(password);
    if (len > 0 && password[len - 1] == '\n')
        password[len - 1] = '\0';

    SetConsoleMode(hStdin, mode);
    printf("\n");

    return password;
}

void press_to_exit() {
    printf("Press any character to exit...");
    getchar();
}
