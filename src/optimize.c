#include "optimize.h"

#include <stdlib.h>

#include "gen_ir.h"
#include "vector.h"

struct Vector *optimize(struct Vector *irs) {
    struct Vector *optimized = vector_new();
    int i = 0;
    while (i < irs->size) {
        struct Ir *ir = (struct Ir *)irs->data[i];
        if (ir->instruction == IR_PUSH) {
            if (i + 1 != irs->size) {
                struct Ir *next_ir = (struct Ir *)irs->data[i + 1];
                if (next_ir->instruction == IR_POP && is_operand_eq(ir->op1, next_ir->op1)) {
                    // If there are consecutive push and pop instructions w.r.t. the same register,
                    // skip both.
                    i += 2;
                    continue;
                }
            }
        }
        vector_push(optimized, ir);
        i++;
    }

    return optimized;
}
