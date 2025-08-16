#include <gtest/gtest.h>
extern "C" {
#include "lexer.h"
#include "parser.h"
}

class ParserTest : public ::testing::Test {
   protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(ParserTest, ParseEmptyProgram) {
    Lexer *lexer = lexer_new("");
    ASSERT_NE(lexer, nullptr);

    Parser *parser = parser_new(lexer);
    ASSERT_NE(parser, nullptr);

    ASTNode *ast = parser_parse(parser);
    ASSERT_NE(ast, nullptr);
    EXPECT_EQ(ast->type, AST_PROGRAM);
    EXPECT_EQ(ast->data.program.function_count, 0);

    ast_node_free(ast);
    parser_free(parser);
    lexer_free(lexer);
}

TEST_F(ParserTest, ParseNumber) {
    Lexer *lexer = lexer_new("42");
    ASSERT_NE(lexer, nullptr);

    Parser *parser = parser_new(lexer);
    ASSERT_NE(parser, nullptr);

    ASTNode *ast = parser_parse(parser);
    ASSERT_NE(ast, nullptr);

    ast_node_free(ast);
    parser_free(parser);
    lexer_free(lexer);
}