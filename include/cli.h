#ifndef CLI_H
#define CLI_H

typedef enum {
    ACTION_INVALID,
    ACTION_EXIT,
    ACTION_ENCRYPT,
    ACTION_DECRYPT,
} cli_action_t;

typedef enum {
    CLI_FLAG_NONE = 0,
    CLI_FLAG_CONTEXT_MENU = 1 << 1,
    CLI_FLAG_VERBOSE = 1 << 2,
} cli_flags_t;

typedef struct {
    const char *file;
    cli_flags_t flags;
} cli_args_t;

cli_action_t parse_cli(int argc, char *argv[], cli_args_t *args);


#endif