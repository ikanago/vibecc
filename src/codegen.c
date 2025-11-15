#include "codegen.h"

#include "parser.h"

static void generate_node(struct AstNode *node);

// ARM64 requires a value of sp is aligned to 16 bytes
static int round_up_16_bytes(int offset) {
    int t = (offset + 16 - 1) / 16;
    return 16 * t;
}

static void generate_integer(struct AstNode *node) {
    printf("  mov w0, %d\n", node->data.integer);
    printf("  str w0, [sp, #-16]!\n");
}

static void generate_identifier(struct AstNode *node) {
    // Use an appropriate register for a type of the identifer
    printf("  ldr w0, [x29, #-%d]\n", node->data.identifer.offset);
    printf("  str w0, [sp, #-16]!\n");
}

static void generate_binary_operation(struct AstNode *node) {
    generate_node(node->data.binary_op.lhs);
    generate_node(node->data.binary_op.rhs);
    printf("  ldr w0, [sp], #16\n");
    printf("  ldr w1, [sp], #16\n");

    switch (node->data.binary_op.op) {
        case TOKEN_PLUS:
            printf("  add w0, w0, w1\n");
            break;
        case TOKEN_MULTIPLICATION:
            printf("  mul w0, w0, w1\n");
            break;
        default:
            break;
    }

    printf("  str w0, [sp, #-16]!\n");
}

static void generate_declaration(struct AstNode *node) {
    generate_node(node->data.declaration.initializer);
    printf("  ldr w0, [sp], #16\n");
    printf("  str w0, [x29, #-%d]\n", node->data.declaration.offset);
}

static void generate_compound_statement(struct AstNode *node) {
    int offset = round_up_16_bytes(node->data.compound_statement.max_variable_offset);
    printf("  sub sp, sp, %d\n", offset);

    struct Vector *block_items = node->data.compound_statement.block_items;
    for (int i = 0; i < block_items->size; i++) {
        struct AstNode *block_item = block_items->data[i];
        generate_node(block_item);
    }
}

static void generate_return_statement(struct AstNode *node) {
    generate_node(node->data.return_statement.exp);
    // Is it OK to pop here even if exp is NULL?
    printf("  ldr w0, [sp], #16\n");
    printf("  b .Lreturn\n");
}

static void generate_node(struct AstNode *node) {
    if (node == NULL) {
        return;
    }

    switch (node->kind) {
        case AST_INTEGER:
            generate_integer(node);
            break;
        case AST_IDENTIFIER:
            generate_identifier(node);
            break;
        case AST_BINARY_OPERATION:
            generate_binary_operation(node);
            break;
        case AST_DECLARATION:
            generate_declaration(node);
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
    printf(".global main\n");
    printf("main:\n");
    printf("  stp x29, x30, [sp, #-16]!\n");
    printf("  mov x29, sp\n");
    generate_node(node);
    printf(".Lreturn:\n");
    printf("  mov sp, x29\n");
    printf("  ldp x29, x30, [sp], #16\n");
    printf("  ret\n");
}
