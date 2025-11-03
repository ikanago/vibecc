#include "util.h"

#include <stdio.h>

#include "parser.h"

static void print_node_inner(struct AstNode *node, int level) {
    for (int i = 0; i < level; i++) {
        printf(" ");
    }

    switch (node->type) {
        case AST_INTEGER:
            printf("Integer: %d", node->data.integer);
            // TODO: output line number and column number, which requires them
            // to be included in AstNode
            break;
        default:
            printf("N/A");
            break;
    }
    printf("\n");
}

void print_node(struct AstNode *node) { print_node_inner(node, 0); }
