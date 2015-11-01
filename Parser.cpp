//
//  Parser.cpp
//  Compiler
//
//  Created by Yuanqi on 10/30/15.
//  Copyright © 2015 Yuanqi. All rights reserved.
//

#include "Parser.hpp"

/* * * * * *
 *
 * Base class of Parser
 *
 * * * * * */

Parser::Parser(Lexer &source, int k)
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
    lookahead[p] = input.nextToken();
    p = (p + 1) % k;
}

Token Parser::lookaheadToken(int i) {
    return lookahead[(p+i-1)%k];
}

int Parser::lookaheadType(int i) {
    return lookaheadToken(i).type();
}

/* * * * * *
 *
 * ListParser
 *
 * * * * * */

ListParser::ListParser(ListLexer &listLexer, int k)
    : Parser(listLexer, k) { }

// [RULE] list: '[' elements ']' ;
void ListParser::list() {
    match(LBRACK_TYPE);
    elements();
    match(RBRACK_TYPE);
}

// [RULE] elements: element (',' element)* ;
void ListParser::elements() {
    element();
    while (lookaheadType(1) == COMMA_TYPE) {
        match(COMMA_TYPE);
        element();
    }
}

// [RULE] element: NAME '=' NAME | NAME | list ;
void ListParser::element() {
    if (lookaheadType(1) == NAME_TYPE && lookaheadType(2) == EQUAL_TYPE) {
        match(NAME_TYPE);
        match(EQUAL_TYPE);
        match(NAME_TYPE);
    } else if (lookaheadType(1) == NAME_TYPE)
        match(NAME_TYPE);
    else if (lookaheadType(1) == LBRACK_TYPE)
        list();
    else
        throw Error("Expecting a NAME, LIST or an assignment.");
}