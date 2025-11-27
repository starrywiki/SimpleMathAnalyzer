/**
 * @file AST.h
 * @brief Defines the Abstract Syntax Tree (AST) node structures.
 */
#ifndef AST_H
#define AST_H

#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include "Lexer.h"

class ASTNode
{
public:
    virtual ~ASTNode() = default;
    // 用于打印树状结构，indent 表示缩进层级
    virtual void print(int indent = 0) const = 0;
};

// 整数节点
class NumberNode : public ASTNode
{
public:
    std::string value;
    explicit NumberNode(std::string val) : value(val) {}

    void print(int indent) const override
    {
        std::cout << std::string(indent * 2, ' ') << "Num: " << value << std::endl;
    }
};

// 变量节点
class VariableNode : public ASTNode
{
public:
    std::string name;
    explicit VariableNode(std::string n) : name(n) {}

    void print(int indent) const override
    {
        std::cout << std::string(indent * 2, ' ') << "Var: " << name << std::endl;
    }
};

// 一元运算符
class UnaryOpNode : public ASTNode
{
public:
    TokenType op;
    std::shared_ptr<ASTNode> left;

    UnaryOpNode(TokenType op, std::shared_ptr<ASTNode> operand)
        : op(op), left(std::move(operand)) {}

    void print(int indent) const override
    {
        std::string opStr;
        switch (op){
        case TokenType::MINUS:
            opStr = "-";
            break;
        }
        std::cout << std::string(indent * 2, ' ') << "UnaryOp: " << opStr << std::endl;
        left->print(indent + 1);
    }
};

// 二元运算符
class BinaryOpNode : public ASTNode
{
public:
    TokenType op;
    std::shared_ptr<ASTNode> left;
    std::shared_ptr<ASTNode> right;

    BinaryOpNode(TokenType op, std::shared_ptr<ASTNode> l, std::shared_ptr<ASTNode> r)
        : op(op), left(std::move(l)), right(std::move(r)) {}

    void print(int indent) const override
    {
        std::string opStr;
        switch (op)
        {
        case TokenType::PLUS:
            opStr = "+";
            break;
        case TokenType::MINUS:
            opStr = "-";
            break;
        case TokenType::MUL:
            opStr = "*";
            break;
        case TokenType::DIV:
            opStr = "/";
            break;
        case TokenType::POW:
            opStr = "^";
            break;
        default:
            opStr = "?";
            break;
        }
        std::cout << std::string(indent * 2, ' ') << "BinaryOp: " << opStr << std::endl;
        left->print(indent + 1);
        right->print(indent + 1);
    }
};

// 一元函数节点
class FunctionNode : public ASTNode
{
public:
    TokenType funcType;
    std::shared_ptr<ASTNode> arg;

    FunctionNode(TokenType type, std::shared_ptr<ASTNode> argument)
        : funcType(type), arg(std::move(argument)) {}

    void print(int indent) const override
    {
        std::string funcName;
        switch (funcType)
        {
        case TokenType::SIN:
            funcName = "sin";
            break;
        case TokenType::COS:
            funcName = "cos";
            break;
        case TokenType::TAN:
            funcName = "tan";
            break;
        case TokenType::COT:
            funcName = "cot";
            break;
        case TokenType::LN:
            funcName = "ln";
            break;
        case TokenType::SQRT:
            funcName = "sqrt";
            break;
        default:
            funcName = "func";
            break;
        }
        std::cout << std::string(indent * 2, ' ') << "Function: " << funcName << std::endl;
        arg->print(indent + 1);
    }
};

#endif