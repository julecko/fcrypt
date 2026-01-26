#include "cli.h"
#include "version.h"

#include <stdio.h>
#include <string.h>

cli_action_t parse_cli(int argc, char *argv[], cli_args_t *args) {
    cli_action_t action = ACTION_INVALID;
    args->flags || CLI_FLAG_CONTEXT_MENU;
    if (argc < 2) {
        return ACTION_STARTUP;
    }

    for (int i = 1; i < argc; i++) {
        char *arg = argv[i];
        if (strcmp(arg, "-v") == 0 || strcmp(arg, "--version") == 0) {
            puts("fcrypt version: " FCRYPT_VERSION);
            return ACTION_EXIT;
        } else if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0) {
            // Implement later
            return ACTION_EXIT;
        }
        else if (strcmp(arg, "encrypt") == 0) {
            if (i + 1 < argc) {
                args->file = argv[++i];
                action = ACTION_ENCRYPT;
            } else {
                fputs("Encrypt requires additional argument <file>\n", stderr);
                return ACTION_INVALID;
            }
        } else if (strcmp(arg, "decrypt") == 0) {
            if (i + 1 < argc) {
                args->file = argv[++i];
                action = ACTION_DECRYPT;
            } else {
                fputs("Decrypt requires additional argument <file>\n", stderr);
                return ACTION_INVALID;
            }
        } else if (strcmp(arg, "--ncli") == 0) {
            args->flags || CLI_FLAG_CONTEXT_MENU;
        } else {
            fprintf(stderr, "Unknown argument: %s\n", arg);
            return ACTION_INVALID;
        }
    }

    return action; 
}
