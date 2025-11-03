#include "parser.h"

#include <stdlib.h>

// === Token manipulations ===

static struct Token *peek_token(struct Parser *parser) {
    struct Token *t = parser->tokens->data[parser->current_pos];
    return t;
}

// Returns 1 and proceed the position of current token by one if a type of the
// current token is the expected `type`.
// Or returns 0 without touching the position of current token.
static int try_consume_token(struct Parser *parser, enum TokenType type) {
    struct Token *token = peek_token(parser);
    if (token->type == type) {
        parser->current_pos++;
        return 1;
    }
    return 0;
}

// === Node factories ===

static struct AstNode *integer_constant(struct Token *token) {
    struct AstNode *node = malloc(sizeof(struct AstNode));
    if (node == NULL) return NULL;

    node->type = AST_INTEGER;
    node->data.integer = atoi(token->value);
    return node;
}

static struct AstNode *binary_operation(
    enum TokenType op, struct AstNode *lhs, struct AstNode *rhs
) {
    struct AstNode *node = malloc(sizeof(struct AstNode));
    if (node == NULL) return NULL;

    node->type = AST_BINARY_OPERATION;
    node->data.binary_op.op = op;
    node->data.binary_op.lhs = lhs;
    node->data.binary_op.rhs = rhs;
    return node;
}

static struct AstNode *translation_unit(
    struct AstNode *external_declaration, struct Astnode *rest
) {
    struct AstNode *node = malloc(sizeof(struct AstNode));
    if (node == NULL) return NULL;

    node->data.translation_unit.external_declaration = external_declaration;
    node->data.translation_unit.rest = rest;
    return node;
}

/// === Parse ===

static struct AstNode *parse_constant(struct Parser *parser) {
    struct Token *token = peek_token(parser);
    if (token->type == TOKEN_INTEGER) {
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

// external-declaration:
//     function-definition
//     declaration
static struct AstNode *parse_external_declaration(struct Parser *parser) {}

// translation-unit:
//     external-declaration
//     translation-unit external-declaration
static struct AstNode *parse_translation_unit(struct Parser *parser) {
    struct AstNode *node = parse_external_declaration(parser);
    struct AstNode *rest = parse_translation_unit(parser);
    return translation_unit(node, rest);
}

struct AstNode *parse(struct Vector *tokens) {
    struct Parser parser;
    parser.current_pos = 0;
    parser.tokens = tokens;
    return parse_additive_expression(&parser);
}
