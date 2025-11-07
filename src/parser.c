#include "parser.h"

#include <stdlib.h>

#include "util.h"

static struct AstNode *parse_statement(struct Parser *parser);

// === Token manipulations ===

static struct Token *peek_token(struct Parser *parser) {
    struct Token *t = parser->tokens->data[parser->current_pos];
    return t;
}

// Returns 1 and proceed the position of current token by one if a kind of the
// current token is the expected `kind`.
// Or returns 0 without touching the position of current token.
static int try_consume_token(struct Parser *parser, enum TokenKind kind) {
    struct Token *token = peek_token(parser);
    if (token->kind == kind) {
        parser->current_pos++;
        return 1;
    }
    return 0;
}

static int check_token(struct Parser *parser, enum TokenKind kind) {
    struct Token *token = peek_token(parser);
    if (token->kind == kind) {
        return 1;
    }
    return 0;
}

// === Node factories ===

static struct Type *type(enum TypeKind kind) {
    struct Type *type = malloc(sizeof(struct Type));
    if (type == NULL) return NULL;

    type->kind = kind;
    switch (kind) {
        case TypeInt:
            type->size = 4;
            break;
        default:
            type->size = 0;
            break;
    }
    return type;
}

static struct AstNode *integer_constant(struct Token *token) {
    struct AstNode *node = malloc(sizeof(struct AstNode));
    if (node == NULL) return NULL;

    node->kind = AST_INTEGER;
    node->data.integer = atoi(token->value);
    return node;
}

static struct AstNode *binary_operation(
    enum TokenKind op, struct AstNode *lhs, struct AstNode *rhs
) {
    struct AstNode *node = malloc(sizeof(struct AstNode));
    if (node == NULL) return NULL;

    node->kind = AST_BINARY_OPERATION;
    node->data.binary_op.op = op;
    node->data.binary_op.lhs = lhs;
    node->data.binary_op.rhs = rhs;
    return node;
}

static struct AstNode *declarator(char *identifer) {
    struct AstNode *node = malloc(sizeof(struct AstNode));
    if (node == NULL) return NULL;

    node->kind = AST_DECLARATOR;
    node->data.declarator.identifier = identifer;
    return node;
}

static struct AstNode *declaration(
    struct Type *type, struct AstNode *declarator, struct AstNode *initializer
) {
    struct AstNode *node = malloc(sizeof(struct AstNode));
    if (node == NULL) return NULL;

    node->kind = AST_DECLARATION;
    node->data.declaration.type = type;
    node->data.declaration.declarator = declarator;
    node->data.declaration.initializer = initializer;
    return node;
}

static struct AstNode *compound_statement(struct Vector *block_items) {
    struct AstNode *node = malloc(sizeof(struct AstNode));
    if (node == NULL) return NULL;

    node->kind = AST_COMPOUND_STATEMENT;
    node->data.compound_statement.block_items = block_items;
    return node;
}

static struct AstNode *return_statement(struct AstNode *exp) {
    struct AstNode *node = malloc(sizeof(struct AstNode));
    if (node == NULL) return NULL;

    node->kind = AST_RETURN_STATEMENT;
    node->data.return_statement.exp = exp;
    return node;
}

// static struct AstNode *translation_unit(
//     struct AstNode *external_declaration, struct Astnode *rest
// ) {
//     struct AstNode *node = malloc(sizeof(struct AstNode));
//     if (node == NULL) return NULL;

//     node->data.translation_unit.external_declaration = external_declaration;
//     node->data.translation_unit.rest = rest;
//     return node;
// }

/// === Parse ===

static struct AstNode *parse_constant(struct Parser *parser) {
    struct Token *token = peek_token(parser);
    if (token->kind == TOKEN_INTEGER) {
        parser->current_pos++;
        return integer_constant(token);
    }
    return NULL;
}

// primary-expression:
//     identifier
//     constant
//     string-literal
//     ( expression )
static struct AstNode *parse_primary_expression(struct Parser *parser) {
    struct AstNode *node;
    if ((node = parse_constant(parser))) {
        return node;
    }
    return NULL;
}

// postfix-expression:
//     primary-expression
//     postfix-expression [ expression ]
//     postfix-expression ( argument-expression-list_opt )
//     postfix-expression . identifier
//     postfix-expression -> identifier
//     postfix-expression ++
//     postfix-expression --
//     ( type-name ) { initializer-list }
//     ( type-name ) { initializer-list , }
// argument-expression-list:
//     assignment-expression
//     argument-expression-list , assignment-expression
static struct AstNode *parse_postfix_expression(struct Parser *parser) {
    struct AstNode *node;
    if ((node = parse_primary_expression(parser))) {
        return node;
    }
    return NULL;
}

// unary-expression:
//     postfix-expression
//     ++ unary-expression
//     -- unary-expression
//     unary-operator cast-expression
//     sizeof unary-expression
//     sizeof ( type-name )
// unary-operator: one of
//     & * + - ~ !
static struct AstNode *parse_unary_expression(struct Parser *parser) {
    struct AstNode *node;
    if ((node = parse_postfix_expression(parser))) {
        return node;
    }
    return NULL;
}

// cast-expression:
//     unary-expression
//     ( type-name ) cast-expression
static struct AstNode *parse_cast_expression(struct Parser *parser) {
    struct AstNode *node;
    if ((node = parse_unary_expression(parser))) {
        return node;
    }
    return NULL;
}

// multiplicative-expression:
//     cast-expression
//     multiplicative-expression * cast-expression
//     multiplicative-expression / cast-expression
//     multiplicative-expression % cast-expression
static struct AstNode *parse_multiplicative_expression(struct Parser *parser) {
    struct AstNode *lhs = parse_cast_expression(parser);

    while (try_consume_token(parser, TOKEN_MULTIPLICATION)) {
        struct AstNode *rhs = parse_cast_expression(parser);
        lhs = binary_operation(TOKEN_MULTIPLICATION, lhs, rhs);
    }

    return lhs;
}

// additive-expression:
//     multiplicative-expression
//     additive-expression + multiplicative-expression
//     additive-expression - multiplicative-expression
static struct AstNode *parse_additive_expression(struct Parser *parser) {
    struct AstNode *lhs = parse_multiplicative_expression(parser);

    while (try_consume_token(parser, TOKEN_PLUS)) {
        struct AstNode *rhs = parse_multiplicative_expression(parser);
        lhs = binary_operation(TOKEN_PLUS, lhs, rhs);
    }

    return lhs;
}

// expression:
//     additive-expression
static struct AstNode *parse_expression(struct Parser *parser) {
    return parse_additive_expression(parser);
}

// type-specifier:
//     int
static struct Type *parse_type_specifier(struct Parser *parser) {
    if (try_consume_token(parser, TOKEN_INT)) {
        return type(TypeInt);
    }
    return NULL;
}

// declarator:
//     pointeropt direct-declarator
// direct-declarator:
//     identifier
struct AstNode *parse_declarator(struct Parser *parser) {
    struct Token *token = peek_token(parser);
    if (token->kind == TOKEN_IDENTIFIER) {
        parser->current_pos++;
        return declarator(token->value);
    }
    return NULL;
}

// declaration:
//     declaration-specifiers init-declarator ;
// declaration-specifiers:
//     storage-class-specifier declaration-specifiersopt
//     type-specifier declaration-specifiersopt
//     type-qualifier declaration-specifiersopt
// init-declarator:
//     declarator
//     declarator = initializer
static struct AstNode *parse_declaration(struct Parser *parser) {
    struct Type *type = parse_type_specifier(parser);
    struct AstNode *declarator = parse_declarator(parser);
    try_consume_token(parser, TOKEN_EQ);
    struct AstNode *initializer = parse_constant(parser);
    try_consume_token(parser, TOKEN_SEMICOLON);
    struct AstNode *node = declaration(type, declarator, initializer);
    print_node(node);
    return node;
}

// function-definition:
//     declaration-specifiers declarator declaration-listopt compound-statement
// declaration-list:
//     declaration
//     declaration-list declaration
// static struct AstNode *parse_function_definition(struct Parser *parser) {}

// compound-statement:
//     { block-item-listopt }
// block-item-list:
//     block-item
//     block-item-list block-item
// block-item:
//     declaration
//     statement
static struct AstNode *parse_compound_statement(struct Parser *parser) {
    struct Vector *block_items = vector_new();
    while (!try_consume_token(parser, TOKEN_RBRACE)) {
        vector_push(block_items, parse_statement(parser));
    }
    return compound_statement(block_items);
}

// jump-statement:
//     continue ;
//     break ;
//     return expressionopt ;
static struct AstNode *parse_jump_statement(struct Parser *parser) {
    if (try_consume_token(parser, TOKEN_SEMICOLON)) {
        return return_statement(NULL);
    }

    struct AstNode *exp = parse_expression(parser);
    try_consume_token(parser, TOKEN_SEMICOLON);
    return return_statement(exp);
}

// statement:
//    labeled-statement
//    compound-statement
//    expression-statement
//    selection-statement
//    iteration-statement
//    jump-statement
static struct AstNode *parse_statement(struct Parser *parser) {
    if (try_consume_token(parser, TOKEN_LBRACE)) {
        return parse_compound_statement(parser);
    }
    if (try_consume_token(parser, TOKEN_RETURN)) {
        return parse_jump_statement(parser);
    }
    if (check_token(parser, TOKEN_INT)) {
        return parse_declaration(parser);
    }
    return NULL;
}

// external-declaration:
//     function-definition
//     declaration
// static struct AstNode *parse_external_declaration(struct Parser *parser) {}

// translation-unit:
//     external-declaration
//     translation-unit external-declaration
// static struct AstNode *parse_translation_unit(struct Parser *parser) {
//     struct AstNode *node = parse_external_declaration(parser);
//     struct AstNode *rest = parse_translation_unit(parser);
//     return translation_unit(node, rest);
// }

struct AstNode *parse(struct Vector *tokens) {
    struct Parser parser;
    parser.current_pos = 0;
    parser.tokens = tokens;
    return parse_statement(&parser);
}
