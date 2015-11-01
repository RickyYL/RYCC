//
//  Parser.hpp
//  Compiler
//
//  Created by Yuanqi on 10/30/15.
//  Copyright © 2015 Yuanqi. All rights reserved.
//

//  Created: OCT 30, 2015
//  Last Modified: NOV 11, 2015

//  Modification Logs:
//  OCT 31, 2015:
//      Make class Parser a abstract base class with pure virtual functions.
//      Re-mark some items with correct lables(private/proteced).
//  OCT 31, 2015:
//      Make it a LL(k) parser
//  NOV 1, 2015:
//      Subtract implementations to .cpp file


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
    
    Parser(Lexer &, int);
    virtual ~Parser();
    
    void match(int);
    void consume();
    virtual void parse() = 0;
    
protected:

    Token *lookahead;
    int p = 0;
    
    Token lookaheadToken(int);
    int lookaheadType(int);
    
private:
    
    Lexer &input;
    int k;
};

class ListParser: public Parser {
    
public:
    
    ListParser(ListLexer &, int);
    
    void parse() { list(); }
    
    void list();
    void elements();
    void element();
    
};

#endif /* Parser_hpp */
