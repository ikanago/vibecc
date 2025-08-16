#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef enum {
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
} ASTNodeType;

typedef struct ASTNode {
    ASTNodeType type;
    union {
        struct {
            struct ASTNode **functions;
            size_t function_count;
        } program;

        struct {
            char *name;
            char *return_type;
            struct ASTNode *body;
        } function;

        struct {
            struct ASTNode **statements;
            size_t statement_count;
        } block;

        struct {
            TokenType op;
            struct ASTNode *left;
            struct ASTNode *right;
        } binary_op;

        struct {
            TokenType op;
            struct ASTNode *operand;
        } unary_op;

        struct {
            char *name;
            char *type;
            struct ASTNode *initializer;
        } variable_declaration;

        struct {
            char *name;
            struct ASTNode *value;
        } assignment;

        struct {
            struct ASTNode *value;
        } return_stmt;

        struct {
            struct ASTNode *condition;
            struct ASTNode *then_branch;
            struct ASTNode *else_branch;
        } if_stmt;

        struct {
            struct ASTNode *condition;
            struct ASTNode *body;
        } while_stmt;

        char *identifier;
        int number;
    } data;
} ASTNode;

typedef struct {
    Lexer *lexer;
    Token *current_token;
} Parser;

Parser *parser_new(Lexer *lexer);
void parser_free(Parser *parser);
ASTNode *parser_parse(Parser *parser);
void ast_node_free(ASTNode *node);

#endif
