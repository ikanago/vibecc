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

static void print_type(struct Type *type, int level) {
    for (int i = 0; i < level; i++) {
        printf("  ");
    }

    printf("type: %d (size: %d)\n", type->kind, type->size);
}

static void print_node_inner(struct AstNode *node, int level) {
    for (int i = 0; i < level; i++) {
        printf("  ");
    }

    switch (node->kind) {
        case AST_INTEGER:
            printf("integer: %d\n", node->data.integer);
            // TODO: output line number and column number, which requires them
            // to be included in AstNode
            break;
        case AST_BINARY_OPERATION:
            printf("binary operation: ");
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
        case AST_DECLARATOR:
            printf("declarator: %s\n", node->data.declarator.identifier);
            break;
        case AST_DECLARATION:
            printf("delcaration:\n");
            print_type(node->data.declaration.type, level + 1);
            print_node_inner(node->data.declaration.declarator, level + 1);
            print_node_inner(node->data.declaration.initializer, level + 1);
            break;
        case AST_COMPOUND_STATEMENT:
            printf("compound statement:\n");
            struct Vector *block_items =
                node->data.compound_statement.block_items;
            for (size_t i = 0; i < block_items->size; i++) {
                struct AstNode *block_item = block_items->data[i];
                print_node_inner(block_item, level + 1);
            }
            break;
        case AST_RETURN_STATEMENT:
            printf("return statement:\n");
            if (node->data.return_statement.exp) {
                print_node_inner(node->data.return_statement.exp, level + 1);
            }
            break;
        default:
            printf("N/A\n");
            break;
    }
    // printf("\n");
}

void print_node(struct AstNode *node) { print_node_inner(node, 0); }
