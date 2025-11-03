#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

enum AstNodeType {
    AST_INTEGER,
    AST_BINARY_OPERATION,
};

struct AstNode {
    enum AstNodeType type;
    union {
        int integer;

        struct {
            enum TokenType op;
            struct AstNode *operand;
        } unary_op;

        struct {
            enum TokenType op;
            struct AstNode *lhs;
            struct AstNode *rhs;
        } binary_op;

        struct {
            struct AstNode *external_declaration;
            struct AstNode *rest;
        } translation_unit;
    } data;
};

struct Parser {
    struct Vector *tokens;
    size_t current_pos;
};

struct AstNode *parse(struct Vector *tokens);

#endif
