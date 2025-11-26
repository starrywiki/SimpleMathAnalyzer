/**
 * @file Parser.h
 * @brief Declares the Parser (Syntax Analyzer) class.
 *
 * The Parser takes the Token stream from the Lexer and constructs the Abstract Syntax Tree (AST).
 * It enforces the grammatical rules of the mathematical expressions. The class uses a recursive-descent
 * parsing strategy, with dedicated functions (ParseExpression, ParseTerm, ParseFactor, etc.)
 * to handle operator precedence and associativity correctly.
 */