#include "cli.h"
#include "crypt/crypt.h"

#include <stdio.h>
#include <stdlib.h>
#include <sodium.h>

int main(int argc, char *argv[]) {
    CliArgs args;
    CliAction action = parse_cli(argc, argv, &args);

    switch (action) {
        case ACTION_EXIT:
            return EXIT_SUCCESS;
        case ACTION_INVALID:
            return EXIT_FAILURE;
    }

    if (sodium_init() < 0) {
        puts("libsodium init failed");
        return 1;
    }

    char password[256];
    printf("Enter password: ");
    scanf("%255[^\n]", password);

    switch (action) {
        case ACTION_ENCRYPT:
            encrypt(args.file, password);
            break;
        case ACTION_DECRYPT:
            decrypt(args.file, password);
            break;
    }

    return EXIT_SUCCESS;
}