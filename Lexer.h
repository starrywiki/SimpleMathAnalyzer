/**
 * @file Lexer.h
 * @brief Declares the Lexer (Lexical Analyzer) class.
 *
 * The Lexer is responsible for transforming the raw input string into a stream of Tokens.
 * This class includes methods for advancing through the input stream and recognizing tokens
 * like numbers, variables, operators, and reserved function names (e.g., sin, ln).
 * It also defines the lookahead mechanism (current_token and next_token) necessary for
 * robust handling of implicit multiplication, which is implemented in Lexer.cpp.
 */