#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "codegen.h"
#include "lexer.h"
#include "parser.h"
#include "util.h"

char *read_file(char *file_path) {
    FILE *source = fopen(file_path, "r");
    if (source == NULL) {
        perror(file_path);
        exit(1);
    }

    struct stat file_stat;
    int stat_result = stat(file_path, &file_stat);
    if (stat_result == -1) {
        perror("stat() error");
        exit(1);
    }

    int file_size = file_stat.st_size;
    char *buffer = malloc(file_size + 2);
    fread(buffer, file_size, 1, source);

    // Make sure that the end of code is "\n\0".
    if (file_size == 0 || buffer[file_size - 1] != '\n') {
        buffer[file_size++] = '\n';
    }
    buffer[file_size] = '\0';

    fclose(source);
    return buffer;
}

int main(int argc, char **argv) {
    int should_dump_token = 0;
    int should_dump_node = 0;
    for (int i = 0; i < argc; i++) {
        if (!strcmp(argv[i], "--dump-token")) {
            should_dump_token = 1;
        } else if (!strcmp(argv[i], "--dump-node")) {
            should_dump_node = 1;
        }
    }

    const char *input = read_file(argv[argc - 1]);
    struct Vector *tokens = lex(input);
    if (should_dump_token) {
        print_tokens(tokens);
    }

    struct AstNode *node = parse(tokens);
    if (should_dump_node) {
        print_node(node);
    }

    generate(node);

    return 0;
}
