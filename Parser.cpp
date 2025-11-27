/**
 * @file Parser.cpp
 * @brief Implements the recursive descent parser.
 */
#include "Parser.h"
#include <stdexcept>

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), pos(0) {
    if (!tokens.empty()) {
        current_token = tokens[0];
    } else {
        current_token = Token(TokenType::END_OF_FILE, "");
    }
}

void Parser::advance() {
    pos++;
    if (pos < tokens.size()) {
        current_token = tokens[pos];
    } else {
        current_token = Token(TokenType::END_OF_FILE, "");
    }
}

void Parser::eat(TokenType type) {
    if (current_token.type == type) {
        advance();
    } else {
        throw std::runtime_error("Unexpected token: " + current_token.toString() + 
                                 ", expected type code: " + std::to_string((int)type));
    }
}

std::shared_ptr<ASTNode> Parser::parse() {
    return parse_expression();
}

// Expression: Term ((PLUS | MINUS) Term)*
std::shared_ptr<ASTNode> Parser::parse_expression() {
    auto left = parse_term();

    while (current_token.type == TokenType::PLUS || 
           current_token.type == TokenType::MINUS) {
        TokenType op = current_token.type;
        advance();
        auto right = parse_term();
        left = std::make_shared<BinaryOpNode>(op, std::move(left), std::move(right));
    }

    return left;
}

// Term: Factor ((MUL | DIV) Factor)*
std::shared_ptr<ASTNode> Parser::parse_term() {
    auto left = parse_factor();

    while (current_token.type == TokenType::MUL || 
           current_token.type == TokenType::DIV) {
        TokenType op = current_token.type;
        advance();
        auto right = parse_factor();
        left = std::make_shared<BinaryOpNode>(op, std::move(left), std::move(right));
    }

    return left;
}

// Factor: Primary (^ Factor)? 
// 注意：幂运算通常是右结合的，例如 2^3^4 = 2^(3^4)
std::shared_ptr<ASTNode> Parser::parse_factor() {
    auto left = parse_primary();

    if (current_token.type == TokenType::POW) {
        TokenType op = current_token.type;
        advance();
        auto right = parse_factor(); // 递归调用自身以实现右结合
        return std::make_shared<BinaryOpNode>(op, std::move(left), std::move(right));
    }

    return left;
}

// Primary: INT | VAR | LPAREN Expr RPAREN | Function
std::shared_ptr<ASTNode> Parser::parse_primary() {
    switch (current_token.type) {
        case TokenType::INT: {
            std::string val = current_token.value;
            advance();
            return std::make_shared<NumberNode>(val);
        }
        case TokenType::VAR: {
            std::string val = current_token.value;
            advance();
            return std::make_shared<VariableNode>(val);
        }
        case TokenType::LPAREN: {
            advance(); // eat '('
            auto node = parse_expression();
            eat(TokenType::RPAREN); // eat ')'
            return node;
        }
        // 处理函数: sin, cos, tan, cot, ln, sqrt
        case TokenType::SIN:
        case TokenType::COS:
        case TokenType::TAN:
        case TokenType::COT:
        case TokenType::LN:
        case TokenType::SQRT: {
            TokenType type = current_token.type;
            advance(); // 吃掉函数名
            
            // 检查函数后是否有括号，虽然通常是 sin(x)，但有时数学上 sin x 也合法
            // 这里的实现假定函数作用于紧随其后的 Primary 单元
            // 如果是 sin(x)，parse_primary 会处理括号部分
            auto arg = parse_factor(); // 使用 factor 以支持 sin(x)^2 这种结合 (但通常 sin x ^ 2 意味着 sin(x^2) 或 (sin x)^2 取决于约定，这里让它绑定紧随的因子)
            // 为了安全，更建议函数必须带括号，但为了通用性，这里直接递归解析下一个因子
            
            return std::make_shared<FunctionNode>(type, std::move(arg));
        }
        default:
            throw std::runtime_error("Unexpected token in primary: " + current_token.toString());
    }
}