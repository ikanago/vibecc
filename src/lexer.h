#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>

#include "vector.h"

enum TokenKind {
    TOKEN_EOF,
    TOKEN_INTEGER,
    TOKEN_IDENTIFIER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLICATION,
    TOKEN_DIVIDE,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_SEMICOLON,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_ASSIGN,
    TOKEN_INT,
    TOKEN_RETURN,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_WHILE,
    TOKEN_EQ,
    TOKEN_NE,
    TOKEN_LT,
    TOKEN_LE,
    TOKEN_GT,
    TOKEN_GE,
};

struct Token {
    enum TokenKind kind;
    char *value;
    int line;    // 1-indexed
    int column;  // 1-indexed
};

struct Lexer {
    const char *input;
    size_t position;
    size_t length;
    int line;
    int column;
};

struct Vector *lex(const char *input);

#endif
