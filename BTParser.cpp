
//
//  BTParser.cpp
//  Compiler
//
//  Created by Yuanqi on 11/1/15.
//  Copyright © 2015 Yuanqi. All rights reserved.
//

#include "BTParser.hpp"


BTParser::BTParser(Lexer &source): input(source) {
    lookahead.push_back(source.nextToken());
}

// ensure i tokens before the current position
void BTParser::sync(int i) {
    
    // if there's not enough tokens
    if (p + i > lookahead.size()) {
        
        // get enough tokens from source lexer
        std::size_t n = p + i - lookahead.size();
        for (int j = 0; j < n; j++)
            lookahead.push_back(input.nextToken());
    }
}

Token BTParser::lookaheadToken(int i) {
    sync(i);
    return lookahead[p+i-1];
}

int BTParser::lookaheadType(int i) {
    return lookaheadToken(i).type();
}

void BTParser::match(int x) {
    if (lookaheadType(1) == x)
        consume();
    else
        throw MismatchedTokenException(x, lookaheadType(1));
}

void BTParser::consume() {
    p++;
    if (p == lookahead.size() && !isSpeculating()) {
        p = 0;
        lookahead.clear();
    }
    sync(1);
}

std::size_t BTParser::mark() {
    markers.push_back(p);
    return p;
}

void BTParser::release() {
    std::size_t marker = markers[markers.size()-1];
    markers.erase(markers.end()-1);
    p = marker;
}

bool BTParser::isSpeculating() {
    return markers.size() > 0;
}

// [RULE] stat: list EOF | assign EOF;
void BTParser::stat() {
    if (stat_list()) {          // check if stat is a list
        list();
        match(EOF);
    } else if (stat_assign()) { // check if stat is an assignment
        assign();
        match(EOF);
    } else
        throw NoViableAltException(lookaheadType(1));
}

// [RULE] stat: list EOF
bool BTParser::stat_list() {
    bool success = true;
    mark();
    try { list(); match(EOF); }
    catch (RecognitionException re) { success = false; }
    release();
    return success;
}

// [RULE] stat: assign EOF
bool BTParser::stat_assign() {
    bool success = true;
    mark();
    try { assign(); match(EOF); }
    catch (RecognitionException re) { success = false; }
    release();
    return success;
}

// [RULE] list: '[' elements ']' ;
void BTParser::list() {
    match(LBRACK_TYPE);
    elements();
    match(RBRACK_TYPE);
}

// [RULE] elements: element (',' element)* ;
void BTParser::elements() {
    element();
    while (lookaheadType(1) == COMMA_TYPE) {
        match(COMMA_TYPE);
        element();
    }
}

// [RULE] element: NAME '=' NAME | NAME | list ;
void BTParser::element() {
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

// [RULE] assign: list '=' list;
void BTParser::assign() {
    list();
    match(EQUAL_TYPE);
    list();
}