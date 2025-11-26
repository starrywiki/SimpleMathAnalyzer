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
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
using namespace std;

int main(){
    string expr;
    cout << "Enter a mathematical expression: "<< endl;
    getline(cin, expr);
    Lexer lexer(expr);
    vector<Token> tokens = lexer.tokenize();

    cout << "Tokens: " << endl;
    for (const auto& token : tokens) {
        cout << token.toString() << " " << endl;
    }

    return 0;
}