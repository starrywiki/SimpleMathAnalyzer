/**
 * @file Lexer.cpp
 * @brief Implements the core logic of the Lexical Analyzer.
 *
 * This file details the procedures for character consumption, skipping whitespace, and
 * identifying the different token types (ScanNumber, ScanIdentifier). Crucially, it
 * implements the Consume method, which is the primary interface for the Parser.
 * The Consume method is where the logic for inserting the STAR (*) token for
 * implicit multiplication (e.g., in '3x' or '2(x+1)') is handled.
 */