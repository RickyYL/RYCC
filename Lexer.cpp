//
//  Tokenizer.cpp
//  Compiler
//
//  Created by Yuanqi on 10/30/15.
//  Copyright © 2015 Yuanqi. All rights reserved.
//

//  Created: OCT 30, 2015
//  Last Modified: NOV 1, 2015

//  Modification Logs:
//  NOV 1, 2015:
//      Resolved a logical problem caused by EOF_TYPE and EOF

#include "Lexer.hpp"

/* * * * * *
 *
 *  Base Class of Lexer
 *
 * * * * * */

Lexer::Lexer(std::string source)
    : input(source), index(input.begin()), lookahead(*index) {
}

void Lexer::consume() {
    index++;
    if (index >= input.end())
        lookahead = EOF_TYPE;
    else
        lookahead = *index;
}

void Lexer::match(char expectType) {
    if (lookahead == expectType)
        consume();
    else
        throw MismatchedCharacterException(expectType, lookahead);
}

void Lexer::reset() {
    index = input.begin();
    lookahead = *index;
}

/* * * * * *
 *
 *  ListParser
 *
 * * * * * */

ListLexer::ListLexer(std::string source)
    : Lexer(source) {
}

Token ListLexer::nextToken() {
    
    while (lookahead != EOF) {
        switch (lookahead) {
                
        // for all single char symbols, recongize immediately
            case ' ':
            case '\t':
            case '\r':
            case '\n': WHITE(); continue;
                
            case ',': consume(); return Token(COMMA_TYPE, ",");
            case '[': consume(); return Token(LBRACK_TYPE, "[");
            case ']': consume(); return Token(RBRACK_TYPE, "]");
            case '=': consume(); return Token(EQUAL_TYPE, "=");
                
        // for identifier, let the specific function to recongize
            default :
                if (isalpha(lookahead))
                    return NAME();
                else
                    throw MismatchedCharacterException(NAME_TYPE, lookahead);
        }
    }
    
    return Token(EOF_TYPE, "<EOF>");
}

std::vector<Token> ListLexer::tokenize() {
    
    // to really tokenize all tokens in the source text
    // reset before reading
    
    reset();
    
    std::vector<Token> tokens;
    Token token = nextToken();
    while (token.type() != EOF_TYPE) {
        tokens.push_back(token);
        token = nextToken();
    }
    
    return tokens;
}

Token ListLexer::NAME() {
    std::string buffer;
    do {
        buffer.push_back(lookahead);
        consume();
    } while (isalpha(lookahead));
    return Token(NAME_TYPE, buffer);
}

void ListLexer::WHITE() {
    while (isspace(lookahead))
        consume();
}
