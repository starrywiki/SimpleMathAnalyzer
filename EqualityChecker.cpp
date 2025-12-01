/**
 * @file EqualityChecker.cpp
 * @brief Implements the static method for comparing expressions.
 *
 * The core logic in this file involves calling the Normalize() method on the root nodes
 * of the two input ASTs. The simple equality judgment relies entirely on the
 * successful and deterministic normalization of expressions that are algebraically equivalent
 * under the defined rules (e.g., 1+x == x+1).
 */

// EqualityChecker.cpp

#include "EqualityChecker.h"
#include <algorithm>
#include <typeinfo>

//将标准化后的多项式转为唯一字符串
std::string polyToString(const std::vector<Term>& poly) {
    if (poly.empty()) return "0";

    std::string s = "";
    for (size_t i = 0; i < poly.size(); ++i) {
        const auto& term = poly[i];

        // 正数系数
        if (term.coeff > 0 && i > 0) s += "+";
        // 负数系数
        if (term.coeff < 0) s += "-"; 
        int absCoeff = std::abs(term.coeff);
        std::string termStr = "";
        // 如果系数不是 1/-1，或者没有变量因子，则显示系数
        if (absCoeff != 1 || term.vars.empty()) {
            termStr += std::to_string(absCoeff);
        }
        
        // 连接所有变量因子
        for (const auto& var : term.vars) {
            if (!termStr.empty() && isdigit(termStr.back())) termStr += "*"; // 可选：加乘号
            termStr += var;
        }
        s += termStr;
    }
    
    if (s.empty()) return "0";
    return s;
}

void sortAndMerge(std::vector<Term>& terms) {
    std::sort(terms.begin(), terms.end());
    // 合并同类项
    std::vector<Term> merged;
    for (const auto& term : terms) {
        if (!merged.empty() && merged.back().isSameVars(term)) {
            merged.back().coeff += term.coeff;
        } else {
            merged.push_back(term);
        }
    }
    // 移除系数为0的项
    merged.erase(std::remove_if(merged.begin(), merged.end(),
                                [](const Term& t) { return t.coeff == 0; }),
                 merged.end());
    terms = std::move(merged);
}

std::vector<Term> EqualityChecker::standardize(const std::shared_ptr<ASTNode>& node) {
    std::vector<Term> result;
    if (!node) return result;

    //数字节点
    if (auto n = std::dynamic_pointer_cast<NumberNode>(node)) {//检测具体的指针类型
        Term t;
        t.coeff = std::stoi(n->value);
        // factors 为空
        result.push_back(t);
        return result;
    }

    // 变量节点
    if (auto v = std::dynamic_pointer_cast<VariableNode>(node)) {
        Term t;
        t.coeff = 1;
        t.vars.push_back(v->name);
        result.push_back(t);
        return result;
    }

    // 一元函数节点
    if (auto u = std::dynamic_pointer_cast<UnaryOpNode>(node)) {
        auto argPoly = standardize(u->right);
        if (u->op == TokenType::MINUS) {
            // 取反
            for (auto& term : argPoly) term.coeff *= -1;
        }
        result = argPoly;
        sortAndMerge(result);
        return result;
    }

    // 二元运算节点
    if (auto b = std::dynamic_pointer_cast<BinaryOpNode>(node)) {
        // 递归
        auto leftPoly = standardize(b->left);
        auto rightPoly = standardize(b->right);

        if (b->op == TokenType::PLUS) {
            // 加法：直接合并列表
            result = leftPoly;
            result.insert(result.end(), rightPoly.begin(), rightPoly.end());
        } 
        else if (b->op == TokenType::MINUS) {
            // 减法：右边系数取反再合并
            for (auto& term : rightPoly) term.coeff *= -1;
            result = leftPoly;
            result.insert(result.end(), rightPoly.begin(), rightPoly.end());
        }
        else if (b->op == TokenType::MUL) {
            // (a+b)*(c+d) = ac + ad + bc + bd
            for (const auto& l : leftPoly) {
                for (const auto& r : rightPoly) {
                    Term newTerm;
                    newTerm.coeff = l.coeff * r.coeff;
                    newTerm.vars = l.vars;
                    newTerm.vars.insert(newTerm.vars.end(), r.vars.begin(), r.vars.end());
                    std::sort(newTerm.vars.begin(), newTerm.vars.end());
                    result.push_back(newTerm);
                }
            }
        }
    
        else if (b->op == TokenType::DIV || b->op == TokenType::POW) {
            // 处理除法、幂运算等：分开处理两边
            auto leftPoly = standardize(b->left);
            auto rightPoly = standardize(b->right);
            // 两边处理完了之后统一转化成唯一的字符串表示
            std::string leftStr = polyToString(leftPoly);
            std::string rightStr = polyToString(rightPoly);

            //合成一个新term
            std::string combinedStr;
            if (b->op == TokenType::DIV) {
                combinedStr = "(" + leftStr + ")/(" + rightStr + ")";
            } else { // POW
                combinedStr = "(" + leftStr + ")^(" + rightStr + ")";
            }

            Term t;
            t.coeff = 1;
            t.vars.push_back(combinedStr);
            result.push_back(t);
        }
        // 对整个多项式排序并合并同类项
        sortAndMerge(result); 
        return result;
    }

    // 函数节点 (sin, cos...)
    if (auto f = std::dynamic_pointer_cast<FunctionNode>(node)) {
        auto argPoly = standardize(f->arg);
        std::string argStr = polyToString(argPoly);
        std::string funcName = "";
        if (f->funcType == TokenType::SIN) funcName = "sin";
        else if (f->funcType == TokenType::COS) funcName = "cos";
        else if (f->funcType == TokenType::TAN) funcName = "tan";
        else if (f->funcType == TokenType::COT) funcName = "cot";
        else if (f->funcType == TokenType::LN) funcName = "ln";
        else if (f->funcType == TokenType::SQRT) funcName = "sqrt";
        else funcName = "unknown_func";
        
        Term t;
        t.coeff = 1;
        t.vars.push_back(funcName + "(" + argStr + ")");
        result.push_back(t);
        return result;
    }
    
    // 处理异常请况
    else{
        throw std::runtime_error("Unsupported node type");
    }
}

std::string EqualityChecker::getStandardizedString(const std::shared_ptr<ASTNode>& expr) {
    auto poly = standardize(expr);
    return polyToString(poly);
}

bool EqualityChecker::areEqual(const std::shared_ptr<ASTNode>& expr1, const std::shared_ptr<ASTNode>& expr2) {
    std::string standardizedExpr1 = getStandardizedString(expr1);
    std::string standardizedExpr2 = getStandardizedString(expr2);
    std::cout << "Standardized expressions: " << standardizedExpr1 << " and " << standardizedExpr2 << std::endl;
    return standardizedExpr1 == standardizedExpr2;
}