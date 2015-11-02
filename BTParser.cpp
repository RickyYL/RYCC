
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

void BTParser::match(int tokenType) {
    if (lookaheadType(1) == tokenType)
        consume();
    else
        throw MismatchedTokenException(tokenType, lookaheadType(1));
}

void BTParser::consume() {
    index(index() + 1);
    if (index() == lookahead.size() && !isSpeculating()) {
        index(0);
        lookahead.clear();
    }
    sync(1);
}

// ensure i tokens before the current position
void BTParser::sync(int i) {
    // if there're not enough tokens ahead
    if (index() + i > lookahead.size()) {
        // calculate how many tokens need
        auto n = index() + i - lookahead.size();
        // read sufficient tokens
        for (int j = 0; j < n; j++)
            lookahead.push_back(input.nextToken());
    }
}

void BTParser::mark() {
    markers.push_back(index());
}

void BTParser::release() {
    index(markers[markers.size()-1]);
    markers.pop_back();
}

bool BTParser::isSpeculating() {
    return markers.size() > 0;
}

Token BTParser::lookaheadToken(int i) {
    // make sure enough tokens ahead
    sync(i);
    // return the i-th ahead token
    return lookahead[index() + i - 1];
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

/* * * * * *
 *
 *  Memo Parser
 *
 * * * * * */

MemParser::MemParser(Lexer &source): BTParser(source) { }

bool MemParser::alreadyParsedRule(std::map<std::size_t,int> memoization) {
    
    auto memo = memoization.count(index());
    
    if (memo == 0)
        return false;
    
    std::cout << "parsed list before at index: " << index()
    << "; skip ahead to token index: " << memo
    << ": " << lookahead[memo].text();
    
    if (memo == FAILED)
        throw PreviousParseFailedException();
    
    index(memo);
    
    return true;
}

void MemParser::memoize(std::map<std::size_t,int> memoization,
             std::size_t startTokenIndex, bool failed)
{
    std::size_t stopTokenIndex = failed ? FAILED : index();
    memoization.insert({startTokenIndex, stopTokenIndex});
}

void MemParser::list() {
    
    bool failed = false;
    auto startTokenIndex = index();
    
    if (isSpeculating() && alreadyParsedRule(list_memo))
        return;
    
    try { _list(); }
    catch (RecognitionException re) {
        failed = true;
        if (isSpeculating())
            memoize(list_memo, startTokenIndex, failed);
        throw RecognitionException();
    }
    
    if (isSpeculating())
        memoize(list_memo, startTokenIndex, failed);
}

void MemParser::_list() {
    std::cout << "Parse list rule at token index: " << index() << std::endl;
    match(LBRACK_TYPE);
    elements();
    match(RBRACK_TYPE);
}
