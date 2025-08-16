#include <gtest/gtest.h>
#include <sstream>
extern "C" {
#include "codegen.h"
#include "parser.h"
}

class CodegenTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(CodegenTest, GenerateEmptyProgram) {
    ASTNode *program = (ASTNode*)malloc(sizeof(ASTNode));
    program->type = AST_PROGRAM;
    program->data.program.functions = nullptr;
    program->data.program.function_count = 0;
    
    std::ostringstream output;
    FILE *temp_file = tmpfile();
    ASSERT_NE(temp_file, nullptr);
    
    CodeGenerator *codegen = codegen_new(temp_file);
    ASSERT_NE(codegen, nullptr);
    
    int result = codegen_generate(codegen, program);
    EXPECT_EQ(result, 0);
    
    codegen_free(codegen);
    fclose(temp_file);
    ast_node_free(program);
}