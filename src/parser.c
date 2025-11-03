#include "parser.h"

#include <stdlib.h>

static struct Token *peek_token(struct Parser *parser) {
    struct Token *t = parser->tokens->data[parser->current_pos];
    return t;
}

// static int try_consume_token(struct Parser *parser, enum TokenType type) {
//     if (peek_token(parser)->type == type) {
//         parser->current_pos++;
//         return 1;
//     }
//     return 0;
// }

static struct AstNode *integer_constant(struct Token *token) {
    struct AstNode *node = malloc(sizeof(struct AstNode));
    if (node == NULL) return NULL;

    node->type = AST_INTEGER;
    node->data.integer = atoi(token->value);
    return node;
}

static struct AstNode *parse_integer_constant(struct Parser *parser) {
    struct Token *token = peek_token(parser);
    if (token->type == TOKEN_INTEGER) {
        parser->current_pos++;
        return integer_constant(token);
    }
    return NULL;
}

struct AstNode *parse(struct Vector *tokens) {
    struct Parser parser;
    parser.current_pos = 0;
    parser.tokens = tokens;
    return parse_integer_constant(&parser);
}
