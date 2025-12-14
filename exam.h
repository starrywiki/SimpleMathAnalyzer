#ifndef EXAM_H  
#define EXAM_H

#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <ctime>
#include <functional>

class ExpressionGenerator {
private:
    std::mt19937 rng;
    const std::vector<std::string> vars = {"x", "y", "z", "a", "b"};
    const std::vector<std::string> funcs = {"sin", "cos", "tan", "ln", "sqrt"};
    const std::vector<std::string> ops = {"+", "-", "*", "/", "^"};

    // 获取随机整数
    int randomInt(int min, int max) {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(rng);
    }

    // 获取随机浮点数 (0.0 - 1.0)
    double randomDouble() {
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        return dist(rng);
    }

    // 随机选择一个元素
    template<typename T>
    T randomChoice(const std::vector<T>& list) {
        if (list.empty()) return T();
        return list[randomInt(0, list.size() - 1)];
    }

public:
    ExpressionGenerator() {
        rng.seed(std::time(nullptr));
    }

    // 生成基础数值或变量
    std::string generateAtom() {
        if (randomDouble() < 0.6) {
            return std::to_string(randomInt(0, 100)); // 60% 概率是数字
        } else {
            return randomChoice(vars); // 40% 概率是变量
        }
    }

    // 递归生成表达式
    // depth: 当前递归深度
    // maxDepth: 最大允许深度
    std::string generateExpression(int depth, int maxDepth) {
        // 如果达到最大深度，必须返回基础数值，停止递归
        if (depth >= maxDepth) {
            return generateAtom();
        }

        double p = randomDouble();

        // 10% 概率生成括号包裹的表达式: ( expr )
        if (p < 0.1) {
            return "(" + generateExpression(depth + 1, maxDepth) + ")";
        }
        // 15% 概率生成函数调用: func( expr )
        else if (p < 0.25) {
            std::string func = randomChoice(funcs);
            // 为了兼容性，强制函数带括号
            return func + "(" + generateExpression(depth + 1, maxDepth) + ")";
        }
        // 10% 概率生成一元负号: - expr
        else if (p < 0.35) {
            return "-" + generateExpression(depth + 1, maxDepth); // 注意：这需要你的 Parser 支持一元负号
        }
        // 50% 概率生成二元运算: expr op expr
        else if (p < 0.85) {
            std::string left = generateExpression(depth + 1, maxDepth);
            std::string op = randomChoice(ops);
            std::string right = generateExpression(depth + 1, maxDepth);
            return left + " " + op + " " + right;
        }
        // 15% 概率直接返回基础数值
        else {
            return generateAtom();
        }
    }

    // 生成特定类型的边缘测试用例
    std::vector<std::string> generateEdgeCases() {
        return {
            "-5",               // 简单负数
            "-x",               // 负变量
            "--5",              // 双重负号
            "2(x+1)",           // 隐式乘法
            "sin(x)^2",         // 三角函数与幂运算结合
            "sinxlnx",          // 三角函数与 ln 结合
            "sqrt(x^2 + y^2)",  // 复杂嵌套
            "((((x))))",        // 多重括号
            "1 + 2 * 3^4",      // 优先级测试
            "x-yzsinxy",        // 混合运算
            "sinx^2xsiny",      
            "x -y - z"          // 结合性测试
        };
    }
};

#endif