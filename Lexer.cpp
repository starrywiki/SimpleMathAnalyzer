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

Lexer::Lexer(const std::string &text) : text(text), pos(0)
{
    current_char = text.empty() ? '\0' : text[0];
    length = text.length();
}

// pos+1 & read next char
void Lexer::advance()
{
    pos++;
    if (pos < text.length())
    {
        current_char = text[pos];
    }
    else
    {
        current_char = '\0';
    }
}

void Lexer::skip_whitespace()
{
    while (current_char != '\0' && std::isspace(current_char))
    {
        advance();
    }
}

Token Lexer::number()
{
    std::string result;
    while (current_char != '\0' && std::isdigit(current_char))
    {
        result += current_char;
        advance();
    }
    return {TokenType::INT, result};
}

// 优先检查是否是函数关键字，如果是则消耗整个关键字；否则只消耗一个字符作为变量
void Lexer::identifier(std::vector<Token> &tokens)
{
    // define keywords
    static const std::vector<std::pair<std::string, TokenType>> keywords = {
        {"sin", TokenType::SIN},
        {"cos", TokenType::COS},
        {"tan", TokenType::TAN},
        {"cot", TokenType::COT},
        {"ln", TokenType::LN},
        {"sqrt", TokenType::SQRT}};

    // check whether match keywords
    for (const auto &kw : keywords)
    {
        const std::string &name = kw.first;
        size_t len = name.length();

        // 边界检查：剩余长度是否足够
        if (pos + len <= length)
        {
            bool match = true;
            for (size_t i = 0; i < len; ++i)
            {
                if (text[pos + i] != name[i])
                {
                    match = false;
                    break;
                }
            }

            if (match)
            {
                tokens.push_back(Token(kw.second, name));

                for (size_t i = 0; i < len; ++i)
                {
                    advance();
                }
                return;
            }
        }
    }

    // 若所有关键字都没匹配上，那当前字符就是一个变量
    std::string varName(1, current_char);
    tokens.push_back(Token(TokenType::VAR, varName));
    advance();
}

std::vector<Token> Lexer::tokenize()
{
    std::vector<Token> tokens;

    while (current_char != '\0')
    {
        // blank space
        if (std::isspace(current_char))
        {
            skip_whitespace();
            continue;
        }

        // number
        if (std::isdigit(current_char))
        {
            tokens.push_back(number());
            continue;
        }

        // character
        if (std::isalpha(current_char))
        {
            identifier(tokens);
            continue;
        }

        // operators
        switch (current_char)
        {
        case '+':
            tokens.push_back({TokenType::PLUS, "+"});
            break;
        case '-':
            tokens.push_back({TokenType::MINUS, "-"});
            break;
        case '*':
            tokens.push_back({TokenType::MUL, "*"});
            break;
        case '/':
            tokens.push_back({TokenType::DIV, "/"});
            break;
        case '^':
            tokens.push_back({TokenType::POW, "^"});
            break;
        case '(':
            tokens.push_back({TokenType::LPAREN, "("});
            break;
        case ')':
            tokens.push_back({TokenType::RPAREN, ")"});
            break;
        default:
            // throw error
            throw std::runtime_error(std::string("Unknown character: ") + current_char);
        }

        advance();
    }

    tokens.push_back({TokenType::END_OF_FILE, ""});

    // handle implicit mult
    return handle_implicit_multiplication(tokens);
}

std::vector<Token> Lexer::handle_implicit_multiplication(const std::vector<Token> &input_tokens)
{
    std::vector<Token> result;
    if (input_tokens.empty())
        return result;

    result.push_back(input_tokens[0]);

    for (size_t i = 1; i < input_tokens.size(); ++i)
    {
        Token prev = input_tokens[i - 1];
        Token curr = input_tokens[i];

        bool needs_mult = false;

        // 判断前一个 Token 是否是可能触发隐式乘法的“左值”
        bool is_prev_valid = (prev.type == TokenType::INT ||
                              prev.type == TokenType::VAR ||
                              prev.type == TokenType::RPAREN);

        // 判断当前 Token 是否是可能触发隐式乘法的“右值”
        bool is_curr_valid = (curr.type == TokenType::VAR ||
                              curr.type == TokenType::INT ||
                              curr.type == TokenType::LPAREN ||
                              curr.type == TokenType::SIN ||
                              curr.type == TokenType::COS ||
                              curr.type == TokenType::TAN ||
                              curr.type == TokenType::COT ||
                              curr.type == TokenType::LN ||
                              curr.type == TokenType::SQRT);

        if (is_prev_valid && is_curr_valid)
        {
            result.push_back({TokenType::MUL, "*"});
        }

        result.push_back(curr);
    }
    return result;
}