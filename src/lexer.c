#include "lexer.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct Lexer *lexer_new(const char *input) {
    struct Lexer *lexer = malloc(sizeof(struct Lexer));
    if (!lexer) return NULL;

    lexer->input = input;
    lexer->position = 0;
    lexer->length = strlen(input);
    lexer->line = 1;
    lexer->column = 1;

    return lexer;
}

static char peek_char(struct Lexer *lexer) {
    if (lexer->position >= lexer->length) {
        return '\0';
    }
    return lexer->input[lexer->position];
}

static void consume_token(struct Lexer *lexer) {
    if (lexer->position < lexer->length) {
        if (lexer->input[lexer->position] == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
    }
}

static void skip_whitespace(struct Lexer *lexer) {
    while (isspace(peek_char(lexer))) {
        consume_token(lexer);
    }
}

static struct Token *token_new(
    enum TokenKind kind, const char *value, int line, int column
) {
    struct Token *token = malloc(sizeof(struct Token));
    if (!token) return NULL;

    token->kind = kind;
    token->value = value ? strdup(value) : NULL;
    token->line = line;
    token->column = column;

    return token;
}

static struct Token *next_token(struct Lexer *lexer) {
    skip_whitespace(lexer);

    int line = lexer->line;
    int column = lexer->column;
    char current = peek_char(lexer);

    if (current == '\0') {
        return token_new(TOKEN_EOF, "EOF", line, column);
    }

    if (isdigit(current)) {
        size_t start = lexer->position;
        while (isdigit(peek_char(lexer))) {
            consume_token(lexer);
        }
        size_t length = lexer->position - start;
        char *value = malloc(length + 1);
        strncpy(value, lexer->input + start, length);
        value[length] = '\0';
        return token_new(TOKEN_INTEGER, value, line, column);
    }

    if (isalpha(current) || current == '_') {
        size_t start = lexer->position;
        while (isalnum(peek_char(lexer)) || peek_char(lexer) == '_') {
            consume_token(lexer);
        }
        size_t length = lexer->position - start;
        char *value = malloc(length + 1);
        strncpy(value, lexer->input + start, length);
        value[length] = '\0';

        enum TokenKind kind = TOKEN_IDENTIFIER;
        if (strcmp(value, "int") == 0)
            kind = TOKEN_INT;
        else if (strcmp(value, "return") == 0)
            kind = TOKEN_RETURN;
        else if (strcmp(value, "if") == 0)
            kind = TOKEN_IF;
        else if (strcmp(value, "else") == 0)
            kind = TOKEN_ELSE;
        else if (strcmp(value, "while") == 0)
            kind = TOKEN_WHILE;

        return token_new(kind, value, line, column);
    }

    consume_token(lexer);

    switch (current) {
        case '+':
            return token_new(TOKEN_PLUS, NULL, line, column);
        case '-':
            return token_new(TOKEN_MINUS, NULL, line, column);
        case '*':
            return token_new(TOKEN_MULTIPLICATION, NULL, line, column);
        case '/':
            return token_new(TOKEN_DIVIDE, NULL, line, column);
        case '(':
            return token_new(TOKEN_LPAREN, NULL, line, column);
        case ')':
            return token_new(TOKEN_RPAREN, NULL, line, column);
        case ';':
            return token_new(TOKEN_SEMICOLON, NULL, line, column);
        case '{':
            return token_new(TOKEN_LBRACE, NULL, line, column);
        case '}':
            return token_new(TOKEN_RBRACE, NULL, line, column);
        case '=':
            return token_new(TOKEN_ASSIGN, NULL, line, column);
        default:
            return NULL;
    }
}

struct Vector *lex(const char *input) {
    struct Lexer *lexer = lexer_new(input);
    if (lexer == NULL) return NULL;

    struct Vector *tokens = vector_new();
    if (tokens == NULL) return NULL;

    struct Token *token = NULL;
    while ((token = next_token(lexer))) {
        vector_push(tokens, token);
        if (token->kind == TOKEN_EOF) {
            break;
        }
    }

    return tokens;
}
