#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

enum ASTNodeType {
    AST_INTEGER,
};

struct AstNode {
    enum ASTNodeType type;
    union {
        struct {
            enum TokenType op;
            struct AstNode* left;
            struct AstNode* right;
        } binary_op;

        struct {
            enum TokenType op;
            struct AstNode* operand;
        } unary_op;

        int integer;
    } data;
};

struct Parser {
    struct Vector* tokens;
    size_t current_pos;
};

struct AstNode* parse(struct Vector* tokens);

#endif
