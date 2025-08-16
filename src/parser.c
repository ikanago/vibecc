#include "parser.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Parser *parser_new(Lexer *lexer) {
    Parser *parser = malloc(sizeof(Parser));
    if (!parser) return NULL;

    parser->lexer = lexer;
    parser->current_token = lexer_next_token(lexer);

    return parser;
}

void parser_free(Parser *parser) {
    if (parser) {
        if (parser->current_token) {
            token_free(parser->current_token);
        }
        free(parser);
    }
}

static void parser_advance(Parser *parser) {
    if (parser->current_token) {
        token_free(parser->current_token);
    }
    parser->current_token = lexer_next_token(parser->lexer);
}

static bool parser_expect(Parser *parser, TokenType expected) {
    if (parser->current_token->type == expected) {
        parser_advance(parser);
        return true;
    }
    return false;
}

static ASTNode *ast_node_new(ASTNodeType type) {
    ASTNode *node = malloc(sizeof(ASTNode));
    if (!node) return NULL;

    node->type = type;
    memset(&node->data, 0, sizeof(node->data));

    return node;
}

void ast_node_free(ASTNode *node) {
    if (!node) return;

    switch (node->type) {
        case AST_PROGRAM:
            for (size_t i = 0; i < node->data.program.function_count; i++) {
                ast_node_free(node->data.program.functions[i]);
            }
            free(node->data.program.functions);
            break;

        case AST_FUNCTION:
            free(node->data.function.name);
            free(node->data.function.return_type);
            ast_node_free(node->data.function.body);
            break;

        case AST_BLOCK:
            for (size_t i = 0; i < node->data.block.statement_count; i++) {
                ast_node_free(node->data.block.statements[i]);
            }
            free(node->data.block.statements);
            break;

        case AST_BINARY_OP:
            ast_node_free(node->data.binary_op.left);
            ast_node_free(node->data.binary_op.right);
            break;

        case AST_UNARY_OP:
            ast_node_free(node->data.unary_op.operand);
            break;

        case AST_VARIABLE_DECLARATION:
            free(node->data.variable_declaration.name);
            free(node->data.variable_declaration.type);
            ast_node_free(node->data.variable_declaration.initializer);
            break;

        case AST_ASSIGNMENT:
            free(node->data.assignment.name);
            ast_node_free(node->data.assignment.value);
            break;

        case AST_RETURN:
            ast_node_free(node->data.return_stmt.value);
            break;

        case AST_IF:
            ast_node_free(node->data.if_stmt.condition);
            ast_node_free(node->data.if_stmt.then_branch);
            ast_node_free(node->data.if_stmt.else_branch);
            break;

        case AST_WHILE:
            ast_node_free(node->data.while_stmt.condition);
            ast_node_free(node->data.while_stmt.body);
            break;

        case AST_IDENTIFIER:
            free(node->data.identifier);
            break;

        default:
            break;
    }

    free(node);
}

static ASTNode *parse_expression(Parser *parser);

static ASTNode *parse_primary(Parser *parser) {
    if (parser->current_token->type == TOKEN_NUMBER) {
        ASTNode *node = ast_node_new(AST_NUMBER);
        if (!node) return NULL;

        node->data.number = atoi(parser->current_token->value);
        parser_advance(parser);
        return node;
    }

    if (parser->current_token->type == TOKEN_IDENTIFIER) {
        ASTNode *node = ast_node_new(AST_IDENTIFIER);
        if (!node) return NULL;

        node->data.identifier = strdup(parser->current_token->value);
        parser_advance(parser);
        return node;
    }

    if (parser->current_token->type == TOKEN_LPAREN) {
        parser_advance(parser);
        ASTNode *expr = parse_expression(parser);
        parser_expect(parser, TOKEN_RPAREN);
        return expr;
    }

    return NULL;
}

static ASTNode *parse_expression(Parser *parser) {
    return parse_primary(parser);
}

ASTNode *parser_parse(Parser *parser) {
    (void)parser;  // Suppress unused parameter warning
    ASTNode *program = ast_node_new(AST_PROGRAM);
    if (!program) return NULL;

    program->data.program.functions = NULL;
    program->data.program.function_count = 0;

    return program;
}
