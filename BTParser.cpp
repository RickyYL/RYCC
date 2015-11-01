
//
//  BTParser.cpp
//  Compiler
//
//  Created by Yuanqi on 11/1/15.
//  Copyright © 2015 Yuanqi. All rights reserved.
//

//  Created: NOV 1, 2015
//  Last modified: NOV 1, 2015

//  Modification Logs:
//  NOV 1, 2015:
//      Resolved a problem caused by misuse of EOF and EOF_TYPE.

#include "BTParser.hpp"

BTParser::BTParser(Lexer &source): input(source) {
    lookahead.push_back(source.nextToken());
}

/* * * * * *
 *
 *  Helping functions
 *
 * * * * * */

void BTParser::match(int type_name) {
    if (lookaheadType(1) == type_name)
        consume();
    else
        throw MismatchedTokenException(type_name, lookaheadType(1));
}

void BTParser::consume() {
    p++;
    if (p == lookahead.size() && !isSpeculating()) {
        p = 0;
        lookahead.clear();
    }
    sync(1);
}

void BTParser::mark() {
    markers.push_back(p);           // record the current p
}

void BTParser::release() {
    p = markers[markers.size()-1];  // restore p
    markers.pop_back();
}

bool BTParser::isSpeculating() {
    // markers.size() > 0 means that it has some p's values
    // that is, it's undergoing back-tracking parse
    return markers.size() > 0;
}

// ensure i tokens before the current position
void BTParser::sync(int i) {
    if (p + i > lookahead.size()) {                 // if there's not enough tokens
        std::size_t n = p + i - lookahead.size();   // get enough tokens from source lexer
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

/* * * * * *
 *
 *  Parsing Rules
 *
 * * * * * */

// [RULE] stat: list EOF | assign EOF;
void BTParser::stat() {
    // check if stat is a list
    if (stat_list()) {
        list();
        match(EOF_TYPE);
    }
    // check if stat is an assignment
    else if (stat_assign()) {
        assign();
        match(EOF_TYPE);
    } else
        throw NoViableAltException(lookaheadType(1));
}

// [RULE] stat: list EOF
bool BTParser::stat_list() {
    bool success = true;
    mark();         // record where this parsing begun
    try { list(); match(EOF_TYPE); }
    catch (MismatchedTokenException mte) { success = false; }
    release();      // restore to the condition the parsing begun
    return success;
}

// [RULE] stat: assign EOF
bool BTParser::stat_assign() {
    bool success = true;
    mark();
    try { assign(); match(EOF_TYPE); }
    catch (MismatchedTokenException mte) { success = false; }
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