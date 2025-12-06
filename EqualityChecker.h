/**
 * @file EqualityChecker.h
 * @brief Declares the EqualityChecker class.
 *
 * This class provides a static interface for determining the mathematical equivalence
 * of two simple expressions, based on the project requirements. The check is performed
 * by comparing the canonical (normalized) string representations of their respective ASTs.
 */

// filepath: 
#ifndef EQUALITYCHECKER_H
#define EQUALITYCHECKER_H

#include "AST.h"
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>

// 代表多项式中的一项
struct Term {
    int coeff; 
    std::vector<std::string> vars;

    // 排序：先比变量部分，再比系数
    bool operator<(const Term& other) const {
        if (vars != other.vars) return vars < other.vars;
        return coeff < other.coeff;
    }
    
    // 判断变量部分是否相同（用于合并同类项）
    bool isSameVars(const Term& other) const {
        return vars == other.vars;
    }
};

class EqualityChecker {
public:
    static bool areEqual(const std::shared_ptr<ASTNode>& expr1, const std::shared_ptr<ASTNode>& expr2);
    // 返回标准化后的字符串，用于判断是否正确排序以及比较两个表达式是否相等
    static std::string getStandardizedString(const std::shared_ptr<ASTNode>& expr); 
private:
    //将 AST 转换为规范化的多项式形式 (排序后的项列表)
    static std::vector<Term> standardize(const std::shared_ptr<ASTNode>& node);    
};

#endif // EQUALITYCHECKER_H