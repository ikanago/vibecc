#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

enum ASTNodeType {
    AST_PROGRAM,
    AST_FUNCTION,
    AST_STATEMENT,
    AST_EXPRESSION,
    AST_BINARY_OP,
    AST_UNARY_OP,
    AST_NUMBER,
    AST_IDENTIFIER,
    AST_ASSIGNMENT,
    AST_RETURN,
    AST_IF,
    AST_WHILE,
    AST_BLOCK,
    AST_VARIABLE_DECLARATION,
};

struct AstNode {
    enum ASTNodeType type;
    union {
        struct {
            struct AstNode **functions;
            size_t function_count;
        } program;

        struct {
            char *name;
            char *return_type;
            struct AstNode *body;
        } function;

        struct {
            struct AstNode **statements;
            size_t statement_count;
        } block;

        struct {
            enum TokenType op;
            struct AstNode *left;
            struct AstNode *right;
        } binary_op;

        struct {
            enum TokenType op;
            struct AstNode *operand;
        } unary_op;

        struct {
            char *name;
            char *type;
            struct AstNode *initializer;
        } variable_declaration;

        struct {
            char *name;
            struct AstNode *value;
        } assignment;

        struct {
            struct AstNode *value;
        } return_stmt;

        struct {
            struct AstNode *condition;
            struct AstNode *then_branch;
            struct AstNode *else_branch;
        } if_stmt;

        struct {
            struct AstNode *condition;
            struct AstNode *body;
        } while_stmt;

        char *identifier;
        int integer;
    } data;
};

struct Parser {
    struct Vector *tokens;
    size_t current_pos;
};

struct Vector *parse(struct Vector *tokens);

#endif
