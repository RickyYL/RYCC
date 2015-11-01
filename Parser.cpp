//
//  Parser.cpp
//  Compiler
//
//  Created by Yuanqi on 10/30/15.
//  Copyright © 2015 Yuanqi. All rights reserved.
//

#include "Parser.hpp"

Parser::Parser(Lexer *source, int k)
    : input(source), lookahead(new Token[k]), k(k)
{
    for (int i = 0; i < k; i++)
        consume();
}

Parser::~Parser() {
    delete [] lookahead;
}

void Parser::match(int type) {
    if (lookaheadType(1) == type)
        consume();
    else
        throw Error("Invalid token.");
}

void Parser::consume() {
    lookahead[p] = input->nextToken();
    p = (p + 1) % k;
}

Token Parser::lookaheadToken(int i) {
    return lookahead[(p+i-1)%k];
}

int Parser::lookaheadType(int i) {
    return lookaheadToken(i).type();
}



ListParser::ListParser(ListLexer *listLexer, int k)
    : Parser(listLexer, k) { }