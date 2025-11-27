/**
 * @file Parser.h
 * @brief Declares the Parser class for recursive descent parsing.
 */
#ifndef PARSER_H
#define PARSER_H

#include "Lexer.h"
#include "AST.h"
#include <vector>
#include <memory>

class Parser
{
public:
    explicit Parser(const std::vector<Token> &tokens);
    std::shared_ptr<ASTNode> parse();

private:
    const std::vector<Token> &tokens;
    size_t pos;
    Token current_token;

    void advance();
    void eat(TokenType type);

    // 语法规则函数（优先级从低到高)
    std::shared_ptr<ASTNode> parse_expression(); // +, -
    std::shared_ptr<ASTNode> parse_term();       // *, /
    std::shared_ptr<ASTNode> parse_factor();     // ^ (Power)
    std::shared_ptr<ASTNode> parse_primary();    // Number, Var, Parentheses, Functions
};

#endif