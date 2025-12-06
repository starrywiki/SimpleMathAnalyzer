/**
 * @file main.cpp
 * @brief Main entry point and driver program for the Simple Math Expression Analyzer.
 *
 * This file handles user interaction, manages the execution flow, and orchestrates the
 * sequential use of the Lexer, Parser, and EqualityChecker components. It includes
 * routines for single expression analysis (lexing, parsing, evaluation) and
 * two-expression comparison (parsing, equality checking).
 */
#include "Lexer.h"
#include "Parser.h"
#include "exam.h"
#include "EqualityChecker.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <limits>
using namespace std;

// choose whether to enable random_test
#define ENABLE_RANDOM_TEST

shared_ptr<ASTNode> genTokensAST(string expr)
{
    Lexer lexer(expr);
    vector<Token> tokens = lexer.tokenize();
    // Lexical Analysis
    cout << "--- Tokens --- " << endl;
    for (const auto &token : tokens)
    {
        cout << token.toString() << " ";
        cout << endl;
    }
    // Syntax Analysis(Parsing)
    Parser parser(tokens);
    shared_ptr<ASTNode> ast = parser.parse();

    cout << "--- Abstract Syntax Tree (AST) ---" << endl;
    if (ast)
    {
        ast->print(0);
        cout << "--- Standardized Form (SOP) ---" << endl;
        string stdStr1 = EqualityChecker::getStandardizedString(ast);
        cout << stdStr1 << endl;
    }
    return ast;
}

void compare(string expr1, string expr2)
{
    cout << "Comparing the two expressions..." << endl;
    try
    {
        shared_ptr<ASTNode> ast1 = genTokensAST(expr1);
        shared_ptr<ASTNode> ast2 = genTokensAST(expr2);

        if (EqualityChecker::areEqual(ast1, ast2))
        {
            cout << "The two expressions are equal." << endl;
        }
        else
        {
            cout << "The two expressions are not equal." << endl;
        }
    }
    catch (const std::exception &err)
    {
        cerr << "Error: " << err.what() << endl;
    }
}

int main()
{
#ifdef ENABLE_RANDOM_TEST
    // generate expr randomly
    ExpressionGenerator generator;
    string expr;
    cout << "============================================" << endl;
    cout << "   Running Automated Random Tests" << endl;
    cout << "============================================" << endl;

    cout << "=== Random Expressions (Depth 3) ===" << endl;
    for (int i = 0; i < 5; ++i)
    {
        expr = generator.generateExpression(0, 3);
        cout << "Expr " << i + 1 << ": " << expr << endl;
        genTokensAST(expr);
    }

    cout << "\n=== Random Expressions (Depth 5) ===" << endl;
    for (int i = 0; i < 5; ++i)
    {
        expr = generator.generateExpression(0, 5);
        cout << "Expr " << i + 1 << ": " << expr << endl;
        genTokensAST(expr);
    }

    cout << "\n=== Edge Cases ===" << endl;
    auto edges = generator.generateEdgeCases();
    for (const auto &expr : edges)
    {
        cout << "Edge: " << expr << endl;
        genTokensAST(expr);
    }
#endif

    // enter expr manually
    string expr2, expr3;
    int op;

    cout << "============================================" << endl;
    cout << "   Simple Math Analyzer Interactive Mode" << endl;
    cout << "============================================" << endl;
    cout << "Select Operation Mode:" << endl;
    cout << "  [1] Analyze a single expression" << endl;
    cout << "  [2] Compare two expressions" << endl;
    cout << "Enter your choice (1 or 2): ";

    if (!(cin >> op))
    {
        cerr << "Invalid input for operation selection." << endl;
        return 1;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    try
    {
        if (op == 1)
        {
            cout << "Enter a mathematical expression: " << endl;
            getline(cin, expr2);
            genTokensAST(expr2);
        }
        else if (op == 2)
        {
            cout << "Enter the first mathematical expression: " << endl;
            getline(cin, expr2);
            cout << "Enter the second mathematical expression: " << endl;
            getline(cin, expr3);
            compare(expr2, expr3);
        }
        else
        {
            cout << "Invalid option selected." << endl;
        }
    }
    catch (const std::exception &err)
    {
        cerr << "Error: " << err.what() << endl;
    }
    return 0;
}