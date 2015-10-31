//
//  Tokenizer.cpp
//  Compiler
//
//  Created by Yuanqi on 10/30/15.
//  Copyright © 2015 Yuanqi. All rights reserved.
//

#include "Lexer.hpp"

Lexer::Lexer(std::string source)
    : input(source), curr(input.begin()), ch(input[0]) {
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

Token ListLexer::NAME() {
    std::string buffer;
    do {
        buffer.push_back(ch);
        consume();
    } while (isalpha(ch));
    return Token(NAME_TYPE, buffer);
}

Token ListLexer::NUMBER() {
    std::string buffer;
    do {
        buffer.push_back(ch);
        consume();
    } while (isnumber(ch));
    return Token(NUM_TYPE, buffer);
}

void ListLexer::WHITE() {
    while (isspace(ch))
        consume();
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
            case ';': consume(); return Token(SEMIC_TYPE, ";");
            case '[': consume(); return Token(LBRACK_TYPE, "[");
            case ']': consume(); return Token(RBRACK_TYPE, "]");
            case '(': consume(); return Token(LPARE_TYPE, "(");
            case ')': consume(); return Token(RPARE_TYPE, ")");
                
        // for other types, e.g. identifier or name, make the specific function to recongize
            default :
                if (isalpha(ch))
                    return NAME();
                else if (isnumber(ch) || ch == '+' || ch == '-')
                    return NUMBER();
                else throw Error("Invalid character. ");
        }
    }
    return Token(EOF_TYPE, "<EOF>");
}

std::vector<Token> ListLexer::tokenize() {
    std::vector<Token> tokens;
    Token token = nextToken();
    while (token.type() != EOF_TYPE) {
        tokens.push_back(token);
        token = nextToken();
    }
    return tokens;
}

const std::vector<std::string> ListLexer::tokenNames {
    "n/a", "<EOF>", "NAME", "NUMBER",
    "COMMA", "SEMIC", "LBRACK", "RBRACK",
    "LPARE", "RPARE"
};
