#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

Lexer *lexer_new(const char *input) {
    Lexer *lexer = malloc(sizeof(Lexer));
    if (!lexer) return NULL;
    
    lexer->input = input;
    lexer->position = 0;
    lexer->length = strlen(input);
    lexer->line = 1;
    lexer->column = 1;
    
    return lexer;
}

void lexer_free(Lexer *lexer) {
    if (lexer) {
        free(lexer);
    }
}

static char lexer_current_char(Lexer *lexer) {
    if (lexer->position >= lexer->length) {
        return '\0';
    }
    return lexer->input[lexer->position];
}

static void lexer_advance(Lexer *lexer) {
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

static void lexer_skip_whitespace(Lexer *lexer) {
    while (isspace(lexer_current_char(lexer))) {
        lexer_advance(lexer);
    }
}

static Token *token_new(TokenType type, const char *value, int line, int column) {
    Token *token = malloc(sizeof(Token));
    if (!token) return NULL;
    
    token->type = type;
    token->value = value ? strdup(value) : NULL;
    token->line = line;
    token->column = column;
    
    return token;
}

void token_free(Token *token) {
    if (token) {
        free(token->value);
        free(token);
    }
}

Token *lexer_next_token(Lexer *lexer) {
    lexer_skip_whitespace(lexer);
    
    int line = lexer->line;
    int column = lexer->column;
    char current = lexer_current_char(lexer);
    
    if (current == '\0') {
        return token_new(TOKEN_EOF, NULL, line, column);
    }
    
    if (isdigit(current)) {
        size_t start = lexer->position;
        while (isdigit(lexer_current_char(lexer))) {
            lexer_advance(lexer);
        }
        size_t length = lexer->position - start;
        char *value = malloc(length + 1);
        strncpy(value, lexer->input + start, length);
        value[length] = '\0';
        Token *token = token_new(TOKEN_NUMBER, value, line, column);
        free(value);
        return token;
    }
    
    if (isalpha(current) || current == '_') {
        size_t start = lexer->position;
        while (isalnum(lexer_current_char(lexer)) || lexer_current_char(lexer) == '_') {
            lexer_advance(lexer);
        }
        size_t length = lexer->position - start;
        char *value = malloc(length + 1);
        strncpy(value, lexer->input + start, length);
        value[length] = '\0';
        
        TokenType type = TOKEN_IDENTIFIER;
        if (strcmp(value, "int") == 0) type = TOKEN_INT;
        else if (strcmp(value, "return") == 0) type = TOKEN_RETURN;
        else if (strcmp(value, "if") == 0) type = TOKEN_IF;
        else if (strcmp(value, "else") == 0) type = TOKEN_ELSE;
        else if (strcmp(value, "while") == 0) type = TOKEN_WHILE;
        
        Token *token = token_new(type, value, line, column);
        free(value);
        return token;
    }
    
    lexer_advance(lexer);
    
    switch (current) {
        case '+': return token_new(TOKEN_PLUS, NULL, line, column);
        case '-': return token_new(TOKEN_MINUS, NULL, line, column);
        case '*': return token_new(TOKEN_MULTIPLY, NULL, line, column);
        case '/': return token_new(TOKEN_DIVIDE, NULL, line, column);
        case '(': return token_new(TOKEN_LPAREN, NULL, line, column);
        case ')': return token_new(TOKEN_RPAREN, NULL, line, column);
        case ';': return token_new(TOKEN_SEMICOLON, NULL, line, column);
        case '{': return token_new(TOKEN_LBRACE, NULL, line, column);
        case '}': return token_new(TOKEN_RBRACE, NULL, line, column);
        case '=': return token_new(TOKEN_ASSIGN, NULL, line, column);
        default:
            return NULL;
    }
}
