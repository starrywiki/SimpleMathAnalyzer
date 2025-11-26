/**
 * @file AST.cpp
 * @brief Implements the concrete methods for the Abstract Syntax Tree (AST) nodes.
 *
 * This file contains the implementation details for:
 * 1. Constructors for all AST node types (IntNode, VarNode, etc.).
 * 2. The Evaluate method for computing the numerical result of an expression given an environment.
 * 3. The Normalize method, which is critical for the Simple Equality Judgment. This method handles
 * the canonical representation of expressions, including the commutative and associative properties
 * of addition and multiplication, and special expansion of powers (x^2, x^3).
 */