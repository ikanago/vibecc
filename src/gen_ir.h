#ifndef GEN_IR_H
#define GEN_IR_H

#include <stdio.h>

#include "parser.h"

enum Instruction {
    // Stack
    IR_PUSH,
    IR_POP,
    // Move data
    IR_MOV,
    IR_STR,
    IR_LDR,
    // Branch
    IR_B,
    // Arithmetic
    IR_ADD,
    IR_SUB,
    IR_MUL
};

enum OperandKind {
    OP_REGISTER,
    OP_IMMEDIATE,
    OP_LABEL,
};

struct Operand {
    enum OperandKind kind;
    union {
        char *reg;
        int imm;
        char *label;
    } op;
};

struct Ir {
    enum Instruction instruction;
    struct Operand op1;
    struct Operand op2;
    struct Operand op3;
};

int is_operand_eq(struct Operand op1, struct Operand op2);
struct Vector *generate_ir(struct AstNode *node);

#endif
