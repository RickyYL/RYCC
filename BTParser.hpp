//
//  BTParser.hpp
//  Compiler
//
//  Created by Yuanqi on 11/1/15.
//  Copyright © 2015 Yuanqi. All rights reserved.
//

//  Created: NOV 1, 2015
//  Last modified: NOV 1, 2015

//  Modification Logs:
//  NOV 1, 2015:
//      Created file.

#ifndef BTParser_hpp
#define BTParser_hpp

#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <map>

#include "Lexer.hpp"
#include "Token.hpp"
#include "Error.hpp"

// Back-tracking Parser

// Parsing Rule
//    stat      : list EOF
//              | assign EOF
//              ;
//    assign	: list '=' list;
//    list      : '[' elements ']';
//    elements  : element (',' element)*;
//    element	: NAME '=' NAME
//              | NAME
//              | list
//              ;

class BTParser {
    
public:
    
    BTParser(Lexer &);

    void stat();
    void list();
    void elements();
    void element();
    void assign();
    
protected:
    
    void    match(int);
    void    consume();
    
    void    mark();
    void    release();
    bool    isSpeculating();
    
    void    sync(int);
    Token   lookaheadToken(int);
    int     lookaheadType(int);
    
    bool    stat_list();
    bool    stat_assign();
    
    const std::size_t & index() const { return _index; }
    void index(const std::size_t &index) { _index = index; }
    
    std::vector<Token>          lookahead;
    
private:
    
    Lexer                       &input;
    std::size_t                 _index = 0;
    std::vector<std::size_t>    markers;
};

class MemParser: public BTParser {
    
public:
    
    MemParser(Lexer &);
    
    void list();
    
private:
    
    static const int FAILED = -1;
    
    std::map<std::size_t, int> list_memo;
    
    void _list();
    bool alreadyParsedRule(std::map<std::size_t,int>);
    void memoize(std::map<std::size_t,int>, std::size_t, bool);
    
};

#endif /* BTParser_hpp */
