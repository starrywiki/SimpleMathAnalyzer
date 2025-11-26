/**
 * @file Lexer.h
 * @brief Declares the Lexer (Lexical Analyzer) class.
 *
 * The Lexer is responsible for transforming the raw input string into a stream of Tokens.
 * This class includes methods for advancing through the input stream and recognizing tokens
 * like numbers, variables, operators, and reserved function names (e.g., sin, ln).
 * It also defines the lookahead mechanism (current_token and next_token) necessary for
 * robust handling of implicit multiplication, which is implemented in Lexer.cpp.
 */
#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <iostream>

// 这里定义的是“词法单元类型”，不是“语法树节点类型”
enum class TokenType {
    // 数据类
    INT,        // 整数 "123"
    VAR,        // 变量 "x"

    // 运算符类 (对应 syntax.h 中的 Binop/Unop，但这里只是符号)
    PLUS,       // +
    MINUS,      // -
    MUL,        // *
    DIV,        // /
    POW,        // ^
    LN,         // ln
    SIN,
    COS,
    TAN,
    COT,
    SQRT,

    // 标点符号 (syntax.h 里没有的)
    LPAREN,     // (
    RPAREN,     // )
    
    // 控制类
    END_OF_FILE // 结束标记
};

struct Token {
    TokenType type;
    std::string value; // 存储具体的字符

    // 方便调试打印
    std::string toString() const {
        switch (type) {
            case TokenType::INT: return "INT(" + value + ")";
            case TokenType::VAR: return "VAR(" + value + ")";
            case TokenType::PLUS: return "PLUS";
            case TokenType::MUL:  return "MUL";
            case TokenType::DIV:  return "DIV";
            case TokenType::LN:  return "LN";
            case TokenType::LPAREN: return "LPAREN";
            case TokenType::RPAREN: return "RPAREN";
            case TokenType::END_OF_FILE: return "EOF";
            case TokenType::SIN: return "SIN";
            case TokenType::COS: return "COS";
            case TokenType::TAN: return "TAN";
            case TokenType::COT: return "COT";
            case TokenType::SQRT: return "SQRT";
            default: return "TOKEN(" + value + ")";
        }
    }
};

class Lexer {
public:
    explicit Lexer(const std::string& text);
    std::vector<Token> tokenize();

private:
    std::string text;
    size_t pos;
    char current_char;

    void advance();
    void skip_whitespace();
    Token number();
    Token identifier(); // 处理变量和函数
    std::vector<Token> handle_implicit_multiplication(const std::vector<Token>& tokens);
};

#endif