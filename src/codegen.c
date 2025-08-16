#include "codegen.h"

#include <stdio.h>
#include <stdlib.h>

CodeGenerator *codegen_new(FILE *output) {
    CodeGenerator *codegen = malloc(sizeof(CodeGenerator));
    if (!codegen) return NULL;

    codegen->output = output;
    codegen->label_counter = 0;

    return codegen;
}

void codegen_free(CodeGenerator *codegen) {
    if (codegen) {
        free(codegen);
    }
}

static int codegen_generate_node(CodeGenerator *codegen, ASTNode *node) {
    if (!node) return 0;

    switch (node->type) {
        case AST_PROGRAM:
            fprintf(codegen->output, ".intel_syntax noprefix\n");
            fprintf(codegen->output, ".global _start\n\n");

            for (size_t i = 0; i < node->data.program.function_count; i++) {
                if (codegen_generate_node(
                        codegen, node->data.program.functions[i]
                    ) != 0) {
                    return -1;
                }
            }
            break;

        case AST_FUNCTION:
            fprintf(codegen->output, "%s:\n", node->data.function.name);
            if (codegen_generate_node(codegen, node->data.function.body) != 0) {
                return -1;
            }
            break;

        case AST_NUMBER:
            fprintf(codegen->output, "    mov rax, %d\n", node->data.number);
            break;

        case AST_RETURN:
            if (node->data.return_stmt.value) {
                if (codegen_generate_node(
                        codegen, node->data.return_stmt.value
                    ) != 0) {
                    return -1;
                }
            }
            fprintf(codegen->output, "    mov rdi, rax\n");
            fprintf(codegen->output, "    mov rax, 60\n");
            fprintf(codegen->output, "    syscall\n");
            break;

        default:
            break;
    }

    return 0;
}

int codegen_generate(CodeGenerator *codegen, ASTNode *ast) {
    return codegen_generate_node(codegen, ast);
}
