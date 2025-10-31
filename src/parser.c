#include "parser.h"

#include <stdlib.h>

struct Vector *parse(struct Vector *tokens) {
    struct Parser parser;
    parser.current_pos = 0;
    parser.tokens = tokens;
}

static struct Token *peek_token(struct Parser *parser) {
    struct Token *t = parser->tokens->data[parser->current_pos];
    return t;
}

static struct AstNode *integer_constant(struct Token *token) {
    struct AstNode *node = malloc(sizeof(struct AstNode));
    if (node == NULL) return NULL;
    node->data.integer = atoi(token->value);
    return node;
}

static struct AstNode *parse_integer_constant(struct Parser *parser) {
    struct Token *token = peek_token(parser);
    if (token->type == TOKEN_INTEGER) {
        return integer_constant(token);
    }
    return NULL;
}
