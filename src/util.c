#include "util.h"

#include <stddef.h>
#include <stdio.h>

#include "parser.h"

void print_tokens(struct Vector *tokens) {
    for (size_t i = 0; i < tokens->size; i++) {
        struct Token *t = tokens->data[i];
        printf("(%03d, %03d) %s\n", t->line, t->column, t->value);
    }
}
static void print_node_inner(struct AstNode *node, int level) {
    for (int i = 0; i < level; i++) {
        printf("  ");
    }

    switch (node->type) {
        case AST_INTEGER:
            printf("Integer: %d", node->data.integer);
            // TODO: output line number and column number, which requires them
            // to be included in AstNode
            break;
        case AST_BINARY_OPERATION:
            printf("Binary Operation: ");
            switch (node->data.binary_op.op) {
                case TOKEN_PLUS:
                    printf("+\n");
                    break;
                case TOKEN_MINUS:
                    printf("-\n");
                    break;
                case TOKEN_MULTIPLICATION:
                    printf("*\n");
                    break;
                case TOKEN_DIVIDE:
                    printf("/\n");
                    break;
                default:
                    printf("N/A\n");
                    break;
            }
            print_node_inner(node->data.binary_op.lhs, level + 1);
            print_node_inner(node->data.binary_op.rhs, level + 1);
            break;
        default:
            printf("N/A");
            break;
    }
    printf("\n");
}

void print_node(struct AstNode *node) { print_node_inner(node, 0); }
