#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

enum AstNodeKind {
    AST_INTEGER,
    AST_BINARY_OPERATION,
    AST_COMPOUND_STATEMENT,
    AST_RETURN_STATEMENT,
};

struct AstNode {
    enum AstNodeKind kind;
    union {
        int integer;

        struct {
            enum TokenKind op;
            struct AstNode *operand;
        } unary_op;

        struct {
            enum TokenKind op;
            struct AstNode *lhs;
            struct AstNode *rhs;
        } binary_op;

        struct {
            struct Vector *block_items;
        } compound_statement;

        struct {
            struct AstNode *exp;
        } return_statement;

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
