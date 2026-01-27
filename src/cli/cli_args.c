#include "cli/cli_args.h"
#include "version.h"

#include <stdio.h>
#include <stdlib.h>
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

cli_args_t create_cli_args() {
    cli_args_t args = {0};
    args.file_paths_capacity = 4;
    args.file_paths = malloc(args.file_paths_capacity * sizeof(char *));
    args.file_paths_length = 0;
    args.flags = CLI_FLAG_NONE;

    return args;
}

void free_cli_args(cli_args_t *args) {
    // Implement for windows too
    args->file_paths_length = 0;
    args->file_paths_capacity = 0;
    free(args->file_paths);
}

cli_args_action_t parse_cli_args(int argc, char *argv[], cli_args_t *args) {
    cli_args_action_t action = ACTION_INVALID;

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
        } else if (strcmp(arg, "encrypt") == 0) {
            if (i + 1 >= argc) {
                fputs("Encrypt requires additional argument <file>\n", stderr);
                return ACTION_INVALID;
            }
            i = parse_files(i, argc, argv, args);
            action = ACTION_ENCRYPT;
        } else if (strcmp(arg, "decrypt") == 0) {
            if (i + 1 >= argc) {
                fputs("Decrypt requires additional argument <file>\n", stderr);
                return ACTION_INVALID;
            }
            i = parse_files(i, argc, argv, args);
            action = ACTION_DECRYPT;
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

static int parse_files(int i, int argc, char *argv[], cli_args_t *args) {
    while (i + 1 < argc && argv[i+1][0] != '-') {
        i++;
        if (args->file_paths_length >= MAX_FILES_PARSED) {
            printf("Maximum files passed reached: %d\nStopped on file: %s\n", MAX_FILES_PARSED, argv[i]);
        }
        else if (args->file_paths_length == args->file_paths_capacity) {
            args->file_paths_capacity *= 2;
            char **tmp = realloc(args->file_paths, args->file_paths_capacity * sizeof(char *));
            if (!tmp) {
                fputs(stderr, "Realloc failed");
                exit(1);
            }
            args->file_paths = tmp;
        }
        args->file_paths[args->file_paths_length] = argv[i];
    }

    return i;
}
