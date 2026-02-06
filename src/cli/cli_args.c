#include "cli/cli_args.h"
#include "version.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#endif

static void print_help(void) {
    puts("fcrypt usage: fcrypt <flags> [command]");
    puts("Commands:");
    puts("    encrypt <files>");
    puts("    decrypt <files>");
    puts("Flags:");
    puts("    -h/--help showcase this help message");
    puts("    -v/--verbose more verbose output");
    puts("Examples:");
    puts("    fcrypt encrypt file.txt file1.txt");
    puts("    fcrypt encrypt files/*");
    puts("    fcrypt decrypt file.txt.dec");
    puts("    fcrypt -h");
}

cli_args_t create_cli_args(void) {
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

static int check_expand_args(cli_args_t *args, const char *filename) {
    if (args->file_paths_length >= MAX_FILES_PARSED) {
        printf("Maximum files passed reached: %d\nStopped on file: %s\n", MAX_FILES_PARSED, filename);
        return -1;
    }
    else if (args->file_paths_length == args->file_paths_capacity) {
        args->file_paths_capacity *= 2;
        char **tmp = realloc(args->file_paths, args->file_paths_capacity * sizeof(char *));
        if (!tmp) {
            fputs("Realloc failed", stderr);
            exit(1);
        }
        args->file_paths = tmp;
        return 1;
    } else {
        return 0;
    }
}

#ifdef _WIN32

static void expand_asterisk(const char *pattern, cli_args_t *args) {
    WIN32_FIND_DATAA data;
    HANDLE h = FindFirstFileA(pattern, &data);

    if (h == INVALID_HANDLE_VALUE) {
        fputs("Getting all files in directory failed\n", stderr);
        return;
    }

    char dir[MAX_PATH];
    strcpy(dir, pattern);

    char *star = strrchr(dir, '*');
    if (star) *star = '\0';

    do {
        if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            continue;

        size_t full_len = strlen(dir) + strlen(data.cFileName) + 1;
        args->file_paths[args->file_paths_length] = malloc(full_len);
        sprintf(args->file_paths[args->file_paths_length], "%s%s", dir, data.cFileName);
        args->file_paths_length++;

        if (check_expand_args(args, pattern) == -1) {
            return;
        }
    } while (FindNextFileA(h, &data));

    FindClose(h);
}

#endif

static void parse_files(int *i, int argc, char *argv[], cli_args_t *args) {
    while (*i + 1 < argc && argv[++(*i)][0] != '-') {
        const char *arg = argv[*i];
        if (check_expand_args(args, arg) == -1) {
            return;
        }
        #ifdef _WIN32
        char lastchar = argv[*i][strlen(arg) - 1];
        if (lastchar == '*') {
            expand_asterisk(arg, args);
        } else {
            args->file_paths[args->file_paths_length++] = argv[*i];
        }
        #else
        args->file_paths[args->file_paths_length++] = argv[*i];
        #endif
    }
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
            args->command = CLI_COMMAND_ENCRYPT;
            if (i + 1 >= argc) {
                fputs("Encrypt requires additional argument <file>\n", stderr);
                return ACTION_INVALID;
            }
            parse_files(&i, argc, argv, args);
            action = ACTION_CONTINUE;
        } else if (strcmp(arg, "decrypt") == 0) {
            args->command = CLI_COMMAND_DECRYPT;
            if (i + 1 >= argc) {
                fputs("Decrypt requires additional argument <file>\n", stderr);
                return ACTION_INVALID;
            }
            parse_files(&i, argc, argv, args);
            action = ACTION_CONTINUE;
        } else if (strcmp(arg, "--ncli") == 0) { // Remove later
            args->flags |= CLI_FLAG_CONTEXT_MENU;
        } else if (strcmp(arg, "--gui") == 0) {
            args->flags |= CLI_FLAG_GUI;
        } else if (strcmp(arg, "-v") == 0 || strcmp(arg, "--verbose") == 0){
            args->flags |= CLI_FLAG_VERBOSE;
        } else {
            fprintf(stderr, "Unknown argument: %s\n", arg);
            return ACTION_INVALID;
        }
    }

    return action; 
}
