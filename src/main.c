#include "cli_args.h"
#include "cli.h"
#include "crypt/crypt.h"
#include "util.h"
#include "file_process.h"

#include <stdio.h>
#include <stdlib.h>
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

    bool encrypting = (action == ACTION_ENCRYPT);
    bool is_hidden = take_answer("Hide password?");
    char *password = take_password(is_hidden);

    if (args.file[0] == '*') {
        process_all_files("*", password, encrypting);
    } else {
        process_file(args.file, password, encrypting);
    }
    
    free(password);

    if (args.flags & CLI_FLAG_CONTEXT_MENU) {
        press_to_exit();
    }

    return EXIT_SUCCESS;
}
