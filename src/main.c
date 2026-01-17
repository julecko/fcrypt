#include "cli.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    CliArgs args;
    CliAction action = parse_cli(argc, argv, &args);

    printf("%i\n", action);

    return EXIT_SUCCESS;
}