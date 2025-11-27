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
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
using namespace std;

int main()
{
    string expr;
    cout << "Enter a mathematical expression: " << endl;

    if (!getline(cin, expr) || expr.empty())
    {
        return 0;
    }

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
        std::shared_ptr<ASTNode> ast = parser.parse();

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

    return 0;
}