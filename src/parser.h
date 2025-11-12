#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "map.h"

// Scope manages an offset of each local variable in a scope.
struct Scope {
    struct Map *offsets;
    int current_offset;
};

struct Scope *scope_new();
int *scope_add_var(struct Scope *scope, const char *name, int offset);

enum AstNodeKind {
    AST_INTEGER,
    AST_IDENTIFIER,
    AST_BINARY_OPERATION,
    AST_DECLARATOR,
    AST_DECLARATION,
    AST_COMPOUND_STATEMENT,
    AST_RETURN_STATEMENT,
};

struct AstNode {
    enum AstNodeKind kind;
    union {
        int integer;

        struct {
            char *name;
            int offset;
        } identifer;

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
            char *identifier;
        } declarator;

        struct {
            struct Type *type;
            struct AstNode *declarator;
            struct AstNode *initializer;
            int offset;
        } declaration;

        struct {
            struct Vector *block_items;
            struct Scope *scope;
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

enum TypeKind {
    TypeInt,
};

struct Type {
    enum TypeKind kind;
    int size;
};

struct Parser {
    struct Vector *tokens;
    struct Scope *current_scope;
    int current_pos;
};

struct AstNode *parse(struct Vector *tokens);

#endif
