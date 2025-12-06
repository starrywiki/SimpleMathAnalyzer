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
        auto leftPoly = standardize(b->left);
        auto rightPoly = standardize(b->right);

        if (b->op == TokenType::PLUS) {
            result = leftPoly;
            result.insert(result.end(), rightPoly.begin(), rightPoly.end());
        } 
        else if (b->op == TokenType::MINUS) {
            for (auto& term : rightPoly) term.coeff *= -1;
            result = leftPoly;
            result.insert(result.end(), rightPoly.begin(), rightPoly.end());
        }
        else if (b->op == TokenType::MUL) {
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
        else if (b->op == TokenType::POW) {
            // 检查指数是否为整数 2 或 3
            bool expanded = false;
            if (rightPoly.size() == 1 && rightPoly[0].vars.empty()) { 
                int exp = rightPoly[0].coeff;
                if (exp == 2 || exp == 3) {
                    std::vector<Term> currentPoly = leftPoly; // Base^1
                    
                    for (int k = 1; k < exp; ++k) {
                        std::vector<Term> nextPoly;
                        for (const auto& t1 : currentPoly) {
                            for (const auto& t2 : leftPoly) {
                                Term newTerm;
                                newTerm.coeff = t1.coeff * t2.coeff;
                                newTerm.vars = t1.vars;
                                newTerm.vars.insert(newTerm.vars.end(), t2.vars.begin(), t2.vars.end());
                                // 变量排序以保证唯一性
                                std::sort(newTerm.vars.begin(), newTerm.vars.end());
                                nextPoly.push_back(newTerm);
                            }
                        }
                        currentPoly = nextPoly;
                    }
                    result = currentPoly;
                    expanded = true;
                }
            }

            // 如果无法展开回退到字符串拼接
            if (!expanded) {
                std::string leftStr = polyToString(leftPoly);
                std::string rightStr = polyToString(rightPoly);
                std::string combinedStr = "(" + leftStr + ")^(" + rightStr + ")";
                Term t;
                t.coeff = 1;
                t.vars.push_back(combinedStr);
                result.push_back(t);
            }
        }
        else if (b->op == TokenType::DIV) {
             std::string leftStr = polyToString(leftPoly);
             std::string rightStr = polyToString(rightPoly);
             std::string combinedStr = "(" + leftStr + ")/(" + rightStr + ")";
             Term t;
             t.coeff = 1;
             t.vars.push_back(combinedStr);
             result.push_back(t);
        }

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