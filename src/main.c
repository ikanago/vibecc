#include <stdio.h>
#include <stdlib.h>
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
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        exit(1);
    }

    const char *input = read_file(argv[1]);
    struct Vector *tokens = lex(input);
    struct AstNode *node = parse(tokens);
    generate(node);

    return 0;
}
