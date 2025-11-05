#include "codegen.h"

#include "parser.h"

static void generate_node(struct AstNode *node);

static void generate_integer(struct AstNode *node) {
    printf("  push %d\n", node->data.integer);
}

static void genenrate_binary_operation(struct AstNode *node) {
    generate_node(node->data.binary_op.lhs);
    generate_node(node->data.binary_op.rhs);
    printf("  pop rdi\n");
    printf("  pop rax\n");

    switch (node->data.binary_op.op) {
        case TOKEN_PLUS:
            printf("  add rax, rdi\n");
            break;
        case TOKEN_MULTIPLICATION:
            printf("  mul rdi\n");
            break;
        default:
            break;
    }

    printf("  push rax\n");
}

static void generate_return_statement(struct AstNode *node) {
    generate_node(node->data.return_statement.exp);
    printf("  pop rax\n");
    printf("  jmp .Lreturn\n");
}

static void generate_node(struct AstNode *node) {
    if (node == NULL) {
        return;
    }

    switch (node->type) {
        case AST_INTEGER:
            generate_integer(node);
            break;
        case AST_BINARY_OPERATION:
            genenrate_binary_operation(node);
            break;
        case AST_RETURN_STATEMENT:
            generate_return_statement(node);
            break;
        default:
            break;
    }
}

void generate(struct AstNode *node) {
    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");
    printf("  push rbp\n");
    printf("  mov rbp, rsp\n");
    generate_node(node);
    printf(".Lreturn:\n");
    printf("  mov rsp, rbp\n");
    printf("  pop rbp\n");
    printf("  ret\n");
    printf(".section .note.GNU-stack,\"\",@progbits\n");
}
