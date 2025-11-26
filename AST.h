/**
 * @file AST.h
 * @brief Defines the core data structures for the Simple Math Expression Analyzer.
 *
 * This file contains the definitions for:
 * 1. The TokenType enumeration, listing all valid tokens (operators, functions, constants).
 * 2. The Token struct, representing the smallest meaningful unit of the expression.
 * 3. The ASTNode abstract base class and its concrete derived classes (IntNode, VarNode, BinaryOpNode, FunctionNode).
 * These classes form the Abstract Syntax Tree (AST), which is the input for the Equality Checker.
 * 4. ASTNode includes pure virtual methods for expression normalization and evaluation.
 */