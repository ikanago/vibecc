#ifndef CODEGEN_H
#define CODEGEN_H

#include <stdio.h>

#include "parser.h"

typedef struct {
    FILE *output;
    int label_counter;
} CodeGenerator;

CodeGenerator *codegen_new(FILE *output);
void codegen_free(CodeGenerator *codegen);
int codegen_generate(CodeGenerator *codegen, AstNode *ast);

#endif
