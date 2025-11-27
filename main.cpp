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
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
using namespace std;

void test(const string& expr){
    try
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
        }
    }
    catch (const std::exception &e)
    {
        cerr << "Error: " << e.what() << endl;
    }
}
int main()
{
    //generate expr randomly
    ExpressionGenerator generator;
    string expr;
    cout << "=== Random Expressions (Depth 3) ===" << endl;
    for (int i = 0; i < 5; ++i) {
        expr = generator.generateExpression(0, 3);
        cout << "Expr " << i + 1 << ": " << expr << endl;
        test(expr);
    }

    cout << "\n=== Random Expressions (Depth 5) ===" << endl;
    for (int i = 0; i < 5; ++i) {
        expr = generator.generateExpression(0, 5);
        cout << "Expr " << i + 1 << ": " << expr << endl;
        test(expr);
    }

    cout << "\n=== Edge Cases ===" << endl;
    auto edges = generator.generateEdgeCases();
    for (const auto& expr : edges) {
        cout << "Edge: " << expr << endl;
        test(expr);
    }
    
    //enter expr manually
    string expr2;
    cout << "Enter a mathematical expression: " << endl;

    if (!getline(cin, expr2) || expr.empty())
    {
        test(expr2);
        // return 0;
    }
}