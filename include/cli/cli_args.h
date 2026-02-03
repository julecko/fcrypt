#ifndef CLI_ARGS_H
#define CLI_ARGS_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_FILES_PARSED 256

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
    CLI_FLAG_GUI = 1 << 3,
} cli_args_flags_t;

typedef struct {
    char **file_paths;
    size_t file_paths_length;
    size_t file_paths_capacity;
    cli_args_flags_t flags;
} cli_args_t;

cli_args_t create_cli_args();
void free_cli_args(cli_args_t *args);
cli_args_action_t parse_cli_args(int argc, char *argv[], cli_args_t *args);

#ifdef __cplusplus
}
#endif

#endif
