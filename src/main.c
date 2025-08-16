#include <stdio.h>
#include <stdlib.h>

#include "codegen.h"
#include "lexer.h"
#include "parser.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *input_file = argv[1];
    printf("vibecc: compiling %s\n", input_file);

    return EXIT_SUCCESS;
}
