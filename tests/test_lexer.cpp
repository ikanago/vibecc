#include <gtest/gtest.h>
extern "C" {
#include "lexer.h"
}

class LexerTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(LexerTest, TokenizeNumber) {
    Lexer *lexer = lexer_new("42");
    ASSERT_NE(lexer, nullptr);
    
    Token *token = lexer_next_token(lexer);
    ASSERT_NE(token, nullptr);
    EXPECT_EQ(token->type, TOKEN_NUMBER);
    EXPECT_STREQ(token->value, "42");
    
    token_free(token);
    
    token = lexer_next_token(lexer);
    EXPECT_EQ(token->type, TOKEN_EOF);
    
    token_free(token);
    lexer_free(lexer);
}

TEST_F(LexerTest, TokenizeIdentifier) {
    Lexer *lexer = lexer_new("variable");
    ASSERT_NE(lexer, nullptr);
    
    Token *token = lexer_next_token(lexer);
    ASSERT_NE(token, nullptr);
    EXPECT_EQ(token->type, TOKEN_IDENTIFIER);
    EXPECT_STREQ(token->value, "variable");
    
    token_free(token);
    lexer_free(lexer);
}

TEST_F(LexerTest, TokenizeKeywords) {
    Lexer *lexer = lexer_new("int return if else while");
    ASSERT_NE(lexer, nullptr);
    
    Token *token = lexer_next_token(lexer);
    EXPECT_EQ(token->type, TOKEN_INT);
    token_free(token);
    
    token = lexer_next_token(lexer);
    EXPECT_EQ(token->type, TOKEN_RETURN);
    token_free(token);
    
    token = lexer_next_token(lexer);
    EXPECT_EQ(token->type, TOKEN_IF);
    token_free(token);
    
    token = lexer_next_token(lexer);
    EXPECT_EQ(token->type, TOKEN_ELSE);
    token_free(token);
    
    token = lexer_next_token(lexer);
    EXPECT_EQ(token->type, TOKEN_WHILE);
    token_free(token);
    
    lexer_free(lexer);
}

TEST_F(LexerTest, TokenizeOperators) {
    Lexer *lexer = lexer_new("+ - * / = ( ) ; { }");
    ASSERT_NE(lexer, nullptr);
    
    TokenType expected[] = {
        TOKEN_PLUS, TOKEN_MINUS, TOKEN_MULTIPLY, TOKEN_DIVIDE,
        TOKEN_ASSIGN, TOKEN_LPAREN, TOKEN_RPAREN, TOKEN_SEMICOLON,
        TOKEN_LBRACE, TOKEN_RBRACE
    };
    
    for (size_t i = 0; i < sizeof(expected) / sizeof(TokenType); i++) {
        Token *token = lexer_next_token(lexer);
        EXPECT_EQ(token->type, expected[i]);
        token_free(token);
    }
    
    lexer_free(lexer);
}