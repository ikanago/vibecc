#ifndef CODEGEN_H
#define CODEGEN_H

#include "parser.h"
#include <stdio.h>

typedef struct {
    FILE *output;
    int label_counter;
} CodeGenerator;

CodeGenerator *codegen_new(FILE *output);
void codegen_free(CodeGenerator *codegen);
int codegen_generate(CodeGenerator *codegen, ASTNode *ast);

#endif
