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
    // Is it OK to pop here even if exp is NULL?
    printf("  pop rax\n");
    printf("  jmp .Lreturn\n");
}

static void generate_compound_statement(struct AstNode *node) {
    struct Vector *block_items = node->data.compound_statement.block_items;
    for (size_t i = 0; i < block_items->size; i++) {
        struct AstNode *block_item = block_items->data[i];
        generate_node(block_item);
    }
}

static void generate_node(struct AstNode *node) {
    if (node == NULL) {
        return;
    }

    switch (node->kind) {
        case AST_INTEGER:
            generate_integer(node);
            break;
        case AST_BINARY_OPERATION:
            genenrate_binary_operation(node);
            break;
        case AST_COMPOUND_STATEMENT:
            generate_compound_statement(node);
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
