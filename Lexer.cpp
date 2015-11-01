//
//  Tokenizer.cpp
//  Compiler
//
//  Created by Yuanqi on 10/30/15.
//  Copyright © 2015 Yuanqi. All rights reserved.
//

#include "Lexer.hpp"

/******************************\
 *
 *  Base Class of Lexer
 *
\******************************/

Lexer::Lexer(std::string source)
    : input(source), curr(input.begin()), ch(*curr) {
}

void Lexer::consume() {
    curr++;
    if (curr >= input.end())
        ch = EOF;
    else
        ch = *curr;
}

void Lexer::match(char x) {
    if (ch == x)
        consume();
    else
        throw Error("Cannot found expecting character.");
}

void Lexer::reset() {
    curr = input.begin();
    ch = *curr;
}

/******************************\
 *
 *  Lexer for List, ListParser
 *
\******************************/

ListLexer::ListLexer(std::string source)
    : Lexer(source) {
}

Token ListLexer::nextToken() {
    
    while (ch != EOF) {
        switch (ch) {
                
        // for all single char symbols, recongize immediately
            case ' ':
            case '\t':
            case '\r':
            case '\n':WHITE(); continue;
                
            case ',': consume(); return Token(COMMA_TYPE, ",");
            case '[': consume(); return Token(LBRACK_TYPE, "[");
            case ']': consume(); return Token(RBRACK_TYPE, "]");
            case '=': consume(); return Token(EQUAL_TYPE, "=");
                
        // for identifier, let the specific function to recongize
            default :
                if (isalpha(ch))
                    return NAME();
                else
                    throw Error("Invalid character. ");
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

std::string ListLexer::getTokenName(int n) {
    return tokenNames[n];
}

const std::vector<std::string> ListLexer::tokenNames {
    "n/a", "<EOF>", "NAME", "COMMA", "LBRACK", "RBRACK", "EQUAL"
};

Token ListLexer::NAME() {
    std::string buffer;
    do {
        buffer.push_back(ch);
        consume();
    } while (isalpha(ch));
    return Token(NAME_TYPE, buffer);
}

void ListLexer::WHITE() {
    while (isspace(ch))
        consume();
}
