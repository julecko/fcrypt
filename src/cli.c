#include "cli.h"
#include "version.h"

#include <stdio.h>
#include <string.h>

static void print_help() {
    puts("fcrypt usage: fcrypt <flags> [command]");
    puts("Commands:");
    puts("    encrypt <file>");
    puts("    decrypt <file>");
    puts("Flags:");
    puts("    -h/--help showcase this help message");
    puts("    -v/--verbose more verbose output");
    puts("Examples:");
    puts("    fcrypt encrypt file.txt");
    puts("    fcrypt decrypt file.txt.dec");
    puts("    fcrypt -h");
}

cli_action_t parse_cli(int argc, char *argv[], cli_args_t *args) {
    cli_action_t action = ACTION_INVALID;
    args->flags = CLI_FLAG_NONE;
    if (argc < 2) {
        print_help();
        return action;
    }

    for (int i = 1; i < argc; i++) {
        char *arg = argv[i];
        if (strcmp(arg, "-V") == 0 || strcmp(arg, "--version") == 0) {
            puts("fcrypt version: " FCRYPT_VERSION);
            return ACTION_EXIT;
        } else if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0) {
            print_help();
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
            args->flags |= CLI_FLAG_CONTEXT_MENU;
        } else if (strcmp(arg, "-v") == 0 || strcmp(arg, "--verbose") == 0){
            args->flags |= CLI_FLAG_VERBOSE;
        } else {
            fprintf(stderr, "Unknown argument: %s\n", arg);
            return ACTION_INVALID;
        }
    }

    return action; 
}
