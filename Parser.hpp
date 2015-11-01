//
//  Parser.hpp
//  Compiler
//
//  Created by Yuanqi on 10/30/15.
//  Copyright © 2015 Yuanqi. All rights reserved.
//

//  Created: OCT 30, 2015
//  Last Modified: OCT 31, 2015

//  Modification Logs:
//  OCT 31, 2015:
//      Make class Parser a abstract base class with pure virtual functions.
//      Re-mark some items with correct lables(private/proteced).
//  OCT 31, 2015:
//      Make it a LL(k) parser


#ifndef Parser_hpp
#define Parser_hpp

#include <iostream>
#include <vector>

#include "Token.hpp"
#include "Lexer.hpp"
#include "Error.hpp"

//  LL(k) Parser

//  Rule of Parsing
//  list    : '[' elements ']' ;
//  elements: element (',' element)* ;
//  element : NAME '=' NAME
//          | NAME
//          | list
//          ;

class Parser {
    
public:
    
    Parser(Lexer *, int);
    virtual ~Parser();
    
    void match(int);
    void consume();
    virtual void parse() = 0;
    
protected:
    
    Lexer *input;
    Token *lookahead;
    int k;
    int p = 0;
    
    Token lookaheadToken(int);
    int lookaheadType(int);
};

class ListParser: public Parser {
    
public:
    
    ListParser(ListLexer *, int);
    
    void parse() { list(); }
    
private:
    
    // [RULE] list: '[' elements ']' ;
    void list() {
        match(ListLexer::LBRACK_TYPE);
        elements();
        match(ListLexer::RBRACK_TYPE);
    }
    
    // [RULE] elements: element (',' element)* ;
    void elements() {
        element();
        while (lookaheadType(1) == ListLexer::COMMA_TYPE) {
            match(ListLexer::COMMA_TYPE);
            element();
        }
    }
    
    // [RULE] element: NAME '=' NAME | NAME | list ;
    void element() {
        if (lookaheadType(1) == ListLexer::NAME_TYPE &&
            lookaheadType(2) == ListLexer::EQUAL_TYPE) {
            match(ListLexer::NAME_TYPE);
            match(ListLexer::EQUAL_TYPE);
            match(ListLexer::NAME_TYPE);
        } else if (lookaheadType(1) == ListLexer::NAME_TYPE)
            match(ListLexer::NAME_TYPE);
        else if (lookaheadType(1) == ListLexer::LBRACK_TYPE)
            list();
        else
            throw Error("Expecting a NAME, LIST or an assignment.");
    }
};

#endif /* Parser_hpp */
