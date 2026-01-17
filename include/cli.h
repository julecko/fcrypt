#ifndef CLI_H
#define CLI_H

typedef enum {
    ACTION_INVALID,
    ACTION_EXIT,
    ACTION_STARTUP,
    ACTION_ENCRYPT,
    ACTION_DECRYPT,
} CliAction;

typedef struct {
    const char *file;
} CliArgs;

CliAction parse_cli(int argc, char *argv[], CliArgs *args);


#endif