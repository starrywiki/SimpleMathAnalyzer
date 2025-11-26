/**
 * @file Parser.cpp
 * @brief Implements the grammar rules and AST construction logic for the Parser.
 *
 * This file contains the implementation of the recursive-descent parsing functions.
 * It translates the linear sequence of Tokens into the hierarchical structure of the AST,
 * ensuring syntactic validity. The Match function is essential for consuming expected
 * tokens and propagating syntax errors if the input is malformed.
 */

 #include "Paser.h"