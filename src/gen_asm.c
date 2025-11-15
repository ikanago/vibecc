#include "gen_asm.h"

#include "gen_ir.h"
#include "parser.h"

static void genenrate_push(struct Ir *ir) {
    printf("  str %s, [sp, #-16]!\n", ir->op1.op.reg);
}

static void genenrate_pop(struct Ir *ir) {
    printf("  ldr %s, [sp], #16\n", ir->op1.op.reg);
}

static void genenrate_mov(struct Ir *ir) {
    // Handle the case op2 is reg
    // Maybe we need to introduce OperandKind
    printf("  mov %s, #%d\n", ir->op1.op.reg, ir->op2.op.imm);
}

static void genenrate_str(struct Ir *ir) {
    printf("  str %s, [%s, #%d]\n", ir->op1.op.reg, ir->op2.op.reg, ir->op3.op.imm);
}

static void genenrate_ldr(struct Ir *ir) {
    printf("  ldr %s, [%s, #%d]\n", ir->op1.op.reg, ir->op2.op.reg, ir->op3.op.imm);
}

static void genenrate_b(struct Ir *ir) {
    printf("  b %s\n", ir->op1.op.label);
}

static void genenrate_add(struct Ir *ir) {
    if (ir->op3.kind == OP_REGISTER) {
        printf("  add %s, %s, %s\n", ir->op1.op.reg, ir->op2.op.reg, ir->op3.op.reg);
    } else {
        printf("  add %s, %s, #%d\n", ir->op1.op.reg, ir->op2.op.reg, ir->op3.op.imm);
    }
}

static void genenrate_sub(struct Ir *ir) {
    if (ir->op3.kind == OP_REGISTER) {
        printf("  sub %s, %s, %s\n", ir->op1.op.reg, ir->op2.op.reg, ir->op3.op.reg);
    } else {
        printf("  sub %s, %s, #%d\n", ir->op1.op.reg, ir->op2.op.reg, ir->op3.op.imm);
    }
}

static void genenrate_mul(struct Ir *ir) {
    printf("  mul %s, %s, %s\n", ir->op1.op.reg, ir->op2.op.reg, ir->op3.op.reg);
}

void generate_asm(struct Vector *irs) {
    printf(".global main\n");
    printf("main:\n");
    printf("  stp x29, x30, [sp, #-16]!\n");
    printf("  mov x29, sp\n");
    for (int i = 0; i < irs->size; i++) {
        struct Ir *ir = (struct Ir *)irs->data[i];
        if (ir->instruction == IR_PUSH) {
            genenrate_push(ir);
        }
        if (ir->instruction == IR_POP) {
            genenrate_pop(ir);
        }
        if (ir->instruction == IR_MOV) {
            genenrate_mov(ir);
        }
        if (ir->instruction == IR_STR) {
            genenrate_str(ir);
        }
        if (ir->instruction == IR_LDR) {
            genenrate_ldr(ir);
        }
        if (ir->instruction == IR_B) {
            genenrate_b(ir);
        }
        if (ir->instruction == IR_ADD) {
            genenrate_add(ir);
        }
        if (ir->instruction == IR_SUB) {
            genenrate_sub(ir);
        }
        if (ir->instruction == IR_MUL) {
            genenrate_mul(ir);
        }
    }
    printf(".Lreturn:\n");
    printf("  mov sp, x29\n");
    printf("  ldp x29, x30, [sp], #16\n");
    printf("  ret\n");
}
