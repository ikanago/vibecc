#include "gen_ir.h"

#include <stdlib.h>

#include "parser.h"

static void generate_irs(struct Vector *irs, struct AstNode *node);

// === Operand factories ===
static struct Operand reg(char *reg) {
    struct Operand op = {.kind = OP_REGISTER, .op.reg = reg};
    return op;
}

static struct Operand imm(int imm) {
    struct Operand op = {.kind = OP_IMMEDIATE, .op.imm = imm};
    return op;
}

static struct Operand label(char *label) {
    struct Operand op = {.kind = OP_LABEL, .op.label = label};
    return op;
}

// === Ir factories ===
static struct Ir *push(struct Operand op1) {
    struct Ir *ir = calloc(1, sizeof(struct Ir));
    ir->instruction = IR_PUSH;
    ir->op1 = op1;
    return ir;
}

static struct Ir *pop(struct Operand op1) {
    struct Ir *ir = calloc(1, sizeof(struct Ir));
    ir->instruction = IR_POP;
    ir->op1 = op1;
    return ir;
}

static struct Ir *mov(struct Operand op1, struct Operand op2) {
    struct Ir *ir = calloc(1, sizeof(struct Ir));
    ir->instruction = IR_MOV;
    ir->op1 = op1;
    ir->op2 = op2;
    return ir;
}

static struct Ir *str(struct Operand op1, struct Operand op2, struct Operand op3) {
    struct Ir *ir = calloc(1, sizeof(struct Ir));
    ir->instruction = IR_STR;
    ir->op1 = op1;
    ir->op2 = op2;
    ir->op3 = op3;
    return ir;
}

static struct Ir *ldr(struct Operand op1, struct Operand op2, struct Operand op3) {
    struct Ir *ir = calloc(1, sizeof(struct Ir));
    ir->instruction = IR_LDR;
    ir->op1 = op1;
    ir->op2 = op2;
    ir->op3 = op3;
    return ir;
}

static struct Ir *b(struct Operand op1) {
    struct Ir *ir = calloc(1, sizeof(struct Ir));
    ir->instruction = IR_B;
    ir->op1 = op1;
    return ir;
}

static struct Ir *add(struct Operand op1, struct Operand op2, struct Operand op3) {
    struct Ir *ir = calloc(1, sizeof(struct Ir));
    ir->instruction = IR_ADD;
    ir->op1 = op1;
    ir->op2 = op2;
    ir->op3 = op3;
    return ir;
}

static struct Ir *sub(struct Operand op1, struct Operand op2, struct Operand op3) {
    struct Ir *ir = calloc(1, sizeof(struct Ir));
    ir->instruction = IR_SUB;
    ir->op1 = op1;
    ir->op2 = op2;
    ir->op3 = op3;
    return ir;
}

static struct Ir *mul(struct Operand op1, struct Operand op2, struct Operand op3) {
    struct Ir *ir = calloc(1, sizeof(struct Ir));
    ir->instruction = IR_MUL;
    ir->op1 = op1;
    ir->op2 = op2;
    ir->op3 = op3;
    return ir;
}

// ARM64 requires a value of sp is aligned to 16 bytes
static int round_up_16_bytes(int offset) {
    int t = (offset + 16 - 1) / 16;
    return 16 * t;
}

// === Generate IR ===

static void generate_integer(struct Vector *irs, struct AstNode *node) {
    vector_push(irs, mov(reg("w0"), imm(node->data.integer)));
    vector_push(irs, push(reg("w0")));
}

static void generate_identifier(struct Vector *irs, struct AstNode *node) {
    // Use an appropriate register for a type of the identifer
    vector_push(irs, ldr(reg("w0"), reg("x29"), imm(-node->data.identifer.offset)));
    vector_push(irs, push(reg("w0")));
}

static void generate_binary_operation(struct Vector *irs, struct AstNode *node) {
    generate_irs(irs, node->data.binary_op.lhs);
    generate_irs(irs, node->data.binary_op.rhs);
    vector_push(irs, pop(reg("w0")));
    vector_push(irs, pop(reg("w1")));

    switch (node->data.binary_op.op) {
        case TOKEN_PLUS:
            vector_push(irs, add(reg("w0"), reg("w0"), reg("w1")));
            break;
        case TOKEN_MULTIPLICATION:
            vector_push(irs, mul(reg("w0"), reg("w0"), reg("w1")));
            break;
        default:
            break;
    }

    vector_push(irs, push(reg("w0")));
}

static void generate_declaration(struct Vector *irs, struct AstNode *node) {
    generate_irs(irs, node->data.declaration.initializer);
    vector_push(irs, pop(reg("w0")));
    vector_push(irs, str(reg("w0"), reg("x29"), imm(-node->data.declaration.offset)));
}

static void generate_compound_statement(struct Vector *irs, struct AstNode *node) {
    int offset = round_up_16_bytes(node->data.compound_statement.max_variable_offset);
    vector_push(irs, sub(reg("sp"), reg("sp"), imm(offset)));

    struct Vector *block_items = node->data.compound_statement.block_items;
    for (int i = 0; i < block_items->size; i++) {
        struct AstNode *block_item = block_items->data[i];
        generate_irs(irs, block_item);
    }
}

static void generate_return_statement(struct Vector *irs, struct AstNode *node) {
    generate_irs(irs, node->data.return_statement.exp);
    // Is it OK to pop here even if exp is NULL?
    vector_push(irs, pop(reg("w0")));
    vector_push(irs, b(label(".Lreturn")));
}

static void generate_irs(struct Vector *irs, struct AstNode *node) {
    if (node == NULL) {
        return;
    }

    switch (node->kind) {
        case AST_INTEGER:
            generate_integer(irs, node);
            break;
        case AST_IDENTIFIER:
            generate_identifier(irs, node);
            break;
        case AST_BINARY_OPERATION:
            generate_binary_operation(irs, node);
            break;
        case AST_DECLARATION:
            generate_declaration(irs, node);
            break;
        case AST_COMPOUND_STATEMENT:
            generate_compound_statement(irs, node);
            break;
        case AST_RETURN_STATEMENT:
            generate_return_statement(irs, node);
            break;
        default:
            break;
    }
}

struct Vector *generate_ir(struct AstNode *node) {
    struct Vector *irs = vector_new();
    generate_irs(irs, node);
    return irs;
}
