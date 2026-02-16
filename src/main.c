#include "cli/cli_args.h"
#include "cli/cli.h"
#include "crypt/crypt.h"
#include "util.h"
#include "file_process.h"
#include "gui/gui_main.h"
#include "logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <sodium.h>

int gui_main(cli_args_t *args);

int cli_main(cli_args_t *args) {
    #ifdef DEBUG_MODE
    logger_init(stdout, stderr, LOG_DEBUG, LOG_FLAG_COLOR);
    #else
    logger_init(stdout, stderr, LOG_ERROR, LOG_FLAG_NO_LEVEL | LOG_FLAG_NO_TIMESTAMP | LOG_FLAG_COLOR);
    #endif

    if (sodium_init() < 0) {
        puts("libsodium init failed");
        logger_close();
        return 1;
    }

    bool encrypting = (args->command == CLI_COMMAND_ENCRYPT);
    bool is_hidden = take_answer("Hide password?");
    char *password = take_password(is_hidden);

    for (int i = 0; i < args->file_paths_length; i++) {
        process_file(args->file_paths[i], password, encrypting);
    }

    free_cli_args(args);
    free(password);

    if (args->flags & CLI_FLAG_CONTEXT_MENU) {
        press_to_exit();
    }

    logger_close();

    return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {
    cli_args_t args = create_cli_args();
    cli_args_action_t action = parse_cli_args(argc, argv, &args);

    if (action == ACTION_EXIT)
        return EXIT_SUCCESS;
    if (action == ACTION_INVALID)
        return EXIT_FAILURE;

    if (args.flags & CLI_FLAG_GUI) {
        return gui_main(&args);
    } else {
        #ifdef _WIN32
        ensure_console()
        #endif
        
        return cli_main(&args);
    }
}
