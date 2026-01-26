#include "cli_args.h"
#include "cli.h"
#include "crypt/crypt.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <sodium.h>

int main(int argc, char *argv[]) {
    cli_args_t args;
    cli_args_action_t action = parse_cli_args(argc, argv, &args);

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

    bool is_hidden = take_answer("Hide password?");
    char *password = take_password(is_hidden);

    switch (action) {
        case ACTION_ENCRYPT:
            if (args.file[0] == '*') {
                WIN32_FIND_DATAA data;
                HANDLE h = FindFirstFileA("*", &data);

                if (h == INVALID_HANDLE_VALUE) {
                    fputs(stderr, "Getting all files in directory failed\n");
                    break;
                }

                do {
                    if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                        continue;
                    }
                    encrypt(data.cFileName, password);

                    bool delete_old = take_answer("Delete old file?");
                    if (delete_old) {
                        remove_file(data.cFileName);
                    }
                } while (FindNextFileA(h, &data));

                FindClose(h);
                break;
            } else {
                if (!file_exist(args.file)) {
                    fprintf(stderr, "File %s doesn not exist\n", args.file);
                }

                encrypt(args.file, password);
                bool delete_old = take_answer("Delete old file?");
                if (delete_old) {
                    remove_file(args.file);
                }
                break;
            }
        case ACTION_DECRYPT:
            if (args.file[0] == '*') {
                WIN32_FIND_DATAA data;
                HANDLE h = FindFirstFileA("*", &data);

                if (h == INVALID_HANDLE_VALUE) {
                    fputs(stderr, "Getting all files in directory failed\n");
                    break;
                }

                do {
                    if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                        continue;
                    }
                    decrypt(data.cFileName, password);

                    bool delete_old = take_answer("Delete old file?");
                    if (delete_old) {
                        remove_file(data.cFileName);
                    }
                } while (FindNextFileA(h, &data));

                FindClose(h);
                break;
            } else {
                if (!file_exist(args.file)) {
                    fprintf(stderr, "File %s doesn not exist\n", args.file);
                }

                decrypt(args.file, password);
                bool delete_old = take_answer("Delete old file?");
                if (delete_old) {
                    remove_file(args.file);
                }
                break;
            }
    }
    
    free(password);

    if (args.flags & CLI_FLAG_CONTEXT_MENU) {
        press_to_exit();
    }

    return EXIT_SUCCESS;
}