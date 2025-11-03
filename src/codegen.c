#include "codegen.h"

#include "parser.h"

static void generate_integer(struct AstNode* node) {
    printf("  push %d\n", node->data.integer);
}

void generate(struct AstNode* node) {
    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");
    printf("  push rbp\n");
    printf("  mov rbp, rsp\n");
    generate_integer(node);
    printf("  pop rax\n");
    printf("  jmp .Lreturn\n");
    printf(".Lreturn:\n");
    printf("  mov rsp, rbp\n");
    printf("  pop rbp\n");
    printf("  ret\n");
}
