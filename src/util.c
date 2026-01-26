#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

char *make_enc_filename(const char *filename) {
    size_t len = strlen(filename);
    const char *enc_ext = ".enc";

    char *out = malloc(len + strlen(enc_ext) + 1);
    if (!out) return NULL;

    memcpy(out, filename, len);
    memcpy(out + len, enc_ext, strlen(enc_ext) + 1);

    return out;
}

char *make_dec_filename(const char *filename) {
    size_t len = strlen(filename);

    const char *enc_ext = ".enc";
    const char *dec_ext = ".dec";
    size_t enc_ext_len = strlen(enc_ext);
    size_t dec_ext_len = strlen(dec_ext);

    char *out;

    if (len > enc_ext_len && strcmp(filename + len - enc_ext_len, enc_ext) == 0) {
        out = malloc(len - enc_ext_len + 1);
        strncpy(out, filename, len - enc_ext_len);
        out[len - enc_ext_len] = '\0';
    } else {
        out = malloc(len + dec_ext_len + 1);
        strcpy(out, filename);
        strcat(out, dec_ext);
    }

    return out;
}

char *take_password() {
    char choice = 'n';
    printf("Show password? [y/N] ");
    
    choice = getchar();
    while (getchar() != '\n');

    char *password = malloc(256);
    if (!password) return NULL;

    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);

    if (choice != 'y' && choice != 'Y') {
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