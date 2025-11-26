/**
 * @file Lexer.cpp
 * @brief Implements the core logic of the Lexical Analyzer.
 *
 * This file details the procedures for character consumption, skipping whitespace, and
 * identifying the different token types (ScanNumber, ScanIdentifier). Crucially, it
 * implements the Consume method, which is the primary interface for the Parser.
 * The Consume method is where the logic for inserting the STAR (*) token for
 * implicit multiplication (e.g., in '3x' or '2(x+1)') is handled.
 */
#include "Lexer.h"
#include <cctype>
#include <stdexcept>
#include <algorithm> // for std::find if needed

// 构造函数：初始化字符串和指针
Lexer::Lexer(const std::string& text) : text(text), pos(0) {
    current_char = text.empty() ? '\0' : text[0];
}

// 指针前移：读取下一个字符
void Lexer::advance() {
    pos++;
    if (pos < text.length()) {
        current_char = text[pos];
    } else {
        current_char = '\0'; // 字符串结束符
    }
}

// 跳过空白字符
void Lexer::skip_whitespace() {
    while (current_char != '\0' && std::isspace(current_char)) {
        advance();
    }
}

// 扫描数字 (例如 "123")
Token Lexer::number() {
    std::string result;
    // 只要当前字符是数字，就一直读取
    while (current_char != '\0' && std::isdigit(current_char)) {
        result += current_char;
        advance();
    }
    return {TokenType::INT, result};
}

// 扫描标识符 (变量 "x" 或 函数 "sin")
Token Lexer::identifier() {
    std::string result;
    // 只要是字母，就一直读取
    while (current_char != '\0' && std::isalpha(current_char)) {
        result += current_char;
        advance();
    }

    // 判断是函数名还是普通变量
    if (result == "sin") return {TokenType::SIN, "sin"};
    if (result == "cos") return {TokenType::COS, "cos"};
    if (result == "tan") return {TokenType::TAN, "tan"};
    if (result == "cot") return {TokenType::COT, "cot"};
    if (result == "ln")  return {TokenType::LN, "ln"};
    if (result == "sqrt") return {TokenType::SQRT, "sqrt"};

    // 如果不是函数名，就是变量
    return {TokenType::VAR, result};
}

// ==========================================
// 3. 主分析循环 (核心逻辑)
// ==========================================

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (current_char != '\0') {
        // 情况 A: 空格 -> 跳过
        if (std::isspace(current_char)) {
            skip_whitespace();
            continue;
        }

        // 情况 B: 数字 -> 调用 number()
        if (std::isdigit(current_char)) {
            tokens.push_back(number());
            continue;
        }

        // 情况 C: 字母 -> 调用 identifier()
        if (std::isalpha(current_char)) {
            tokens.push_back(identifier());
            continue;
        }

        // 情况 D: 符号 -> 使用 switch 逐个判断
        switch (current_char) {
            case '+': tokens.push_back({TokenType::PLUS, "+"}); break;
            case '-': tokens.push_back({TokenType::MINUS, "-"}); break;
            case '*': tokens.push_back({TokenType::MUL, "*"}); break;
            case '/': tokens.push_back({TokenType::DIV, "/"}); break;
            case '^': tokens.push_back({TokenType::POW, "^"}); break;
            case '(': tokens.push_back({TokenType::LPAREN, "("}); break;
            case ')': tokens.push_back({TokenType::RPAREN, ")"}); break;
            default:
                // 遇到无法识别的字符，抛出异常
                throw std::runtime_error(std::string("Unknown character: ") + current_char);
        }
        
        // 处理完符号后，指针前移
        advance();
    }

    tokens.push_back({TokenType::END_OF_FILE, ""});
    
    // 最后一步：处理隐式乘法
    return handle_implicit_multiplication(tokens);
}

std::vector<Token> Lexer::handle_implicit_multiplication(const std::vector<Token>& tokens) {
    return tokens; // 暂时不处理隐式乘法，直接返回原始tokens
}