#include "cli.h"

#include <stdio.h>
#include <stdlib.h>
#include <sodium.h>

int main(int argc, char *argv[]) {
    if (sodium_init() < 0) {
        puts("libsodium init failed");
        return 1;
    }
    CliArgs args;
    CliAction action = parse_cli(argc, argv, &args);

    switch (action) {
        case ACTION_EXIT:
            return EXIT_SUCCESS;
        case ACTION_INVALID:
            return EXIT_FAILURE;
    }

    char password[256];
    printf("Enter password: ");
    scanf("%255[^\n]", password);
    printf("%s\n", password);

    return EXIT_SUCCESS;
}