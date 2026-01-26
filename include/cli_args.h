#ifndef CLI_ARGS_H
#define CLI_ARGS_H

typedef enum {
    ACTION_INVALID,
    ACTION_EXIT,
    ACTION_ENCRYPT,
    ACTION_DECRYPT,
} cli_args_action_t;

typedef enum {
    CLI_FLAG_NONE = 0,
    CLI_FLAG_CONTEXT_MENU = 1 << 1,
    CLI_FLAG_VERBOSE = 1 << 2,
} cli_args_flags_t;

typedef struct {
    const char *file;
    cli_args_flags_t flags;
} cli_args_t;

cli_args_action_t parse_cli_args(int argc, char *argv[], cli_args_t *args);


#endif