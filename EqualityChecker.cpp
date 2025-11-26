/**
 * @file EqualityChecker.cpp
 * @brief Implements the static method for comparing expressions.
 *
 * The core logic in this file involves calling the Normalize() method on the root nodes
 * of the two input ASTs. The simple equality judgment relies entirely on the
 * successful and deterministic normalization of expressions that are algebraically equivalent
 * under the defined rules (e.g., 1+x == x+1).
 */