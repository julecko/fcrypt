#include "cli/cli_args.h"
#include "cli/cli.h"
#include "crypt/crypt.h"
#include "util.h"
#include "file_process.h"

#include <stdio.h>
#include <stdlib.h>
#include <sodium.h>

int main(int argc, char *argv[]) {
    cli_args_t args = create_cli_args();
    cli_args_action_t action = parse_cli_args(argc, argv, &args);

    for (int i = 0; i < args.file_paths_length; i++) {
        printf("File parsed %s\n", args.file_paths[i]);
    }

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

    for (int i = 0; i < args.file_paths_length; i++) {
        process_file(args.file_paths[i], password, encrypting);
    }

    free_cli_args(&args);
    free(password);

    if (args.flags & CLI_FLAG_CONTEXT_MENU) {
        press_to_exit();
    }

    return EXIT_SUCCESS;
}
